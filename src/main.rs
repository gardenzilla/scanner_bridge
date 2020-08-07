extern crate hyper;
extern crate websocket;

// use hyper::Fresh;
use hyper::Request;
use hyper::Response;
use hyper::Server as HttpServer;
use serde::{Deserialize, Serialize};
use serde_json::json;
use std::io::Write;
use std::sync::mpsc::channel;
use std::thread;
use websocket::sender::Writer;
use websocket::sync::Server;
use websocket::{Message, OwnedMessage};

mod barcode_bridge;
use barcode_bridge::*;

fn main() {
    // Start listening for WebSocket connections
    let ws_server = Server::bind("127.0.0.1:2794").unwrap();
    for connection in ws_server.filter_map(Result::ok) {
        // Spawn a new thread for each connection.
        thread::spawn(|| {
            let (tx, rx) = channel::<Action>();
            let (tx2, rx2) = channel::<Action>();
            let tx_for_bridge = tx.clone();
            let tx_from_bridge = start_barcode_bridge(tx_for_bridge);
            if !connection
                .protocols()
                .contains(&"scannerbridge".to_string())
            {
                connection.reject().unwrap();
                return;
            }

            let mut client = connection.use_protocol("scannerbridge").accept().unwrap();
            let ip = client.peer_addr().unwrap();

            let (mut ws_rx, mut ws_tx) = client.split().unwrap();

            // Websocket read thread
            thread::spawn(move || {
                for message in ws_rx.incoming_messages() {
                    let message = message.unwrap();
                    match &message {
                        OwnedMessage::Close(_) => {
                            // let message = Message::close();
                            tx.send(Action::Close).unwrap();
                            return;
                        }
                        OwnedMessage::Ping(data) => {
                            tx.send(Action::Ping).unwrap();
                        }
                        OwnedMessage::Text(data) => {
                            match &data[..] {
                                // If we have received error action
                                "error" => tx.send(Action::Error).unwrap(),
                                _ => (),
                            };
                            // println!("New message {}", data.to_string());
                        }
                        _ => {
                            tx.send(Action::Other).unwrap();
                        }
                    }
                }
            });

            // Websocket write thread
            thread::spawn(move || {
                for action in rx2 {
                    match action {
                        Action::Close => {
                            let message = Message::close();
                            ws_tx.send_message(&message).unwrap();
                            println!("Connection closed!");
                            return;
                        }
                        Action::Barcode(code) => {
                            ws_tx
                                .send_message(&Message::text(format!("{{\"code\": \"{}\"}}", code)))
                                .unwrap();
                        }
                        _ => {
                            // Doing nothing
                        }
                    }
                }
            });

            // Main process
            let handle_main = thread::spawn(move || {
                for action in rx {
                    match action {
                        Action::Close => {
                            tx2.send(Action::Close).unwrap();
                            return;
                        }
                        Action::Barcode(code) => tx2.send(Action::Barcode(code)).unwrap(),
                        Action::Error => tx_from_bridge.send(BridgeAction::Error).unwrap(),
                        _ => tx2.send(Action::Other).unwrap(),
                    }
                }
            });

            let _ = handle_main.join();
        });
    }
}
