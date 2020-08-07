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

fn main() {
    // Start listening for WebSocket connections
    let ws_server = Server::bind("127.0.0.1:2794").unwrap();

    for connection in ws_server.filter_map(Result::ok) {
        // Spawn a new thread for each connection.
        thread::spawn(|| {
            if !connection
                .protocols()
                .contains(&"scannerbridge".to_string())
            {
                connection.reject().unwrap();
                return;
            }

            let mut client = connection.use_protocol("scannerbridge").accept().unwrap();
            let ip = client.peer_addr().unwrap();

            // println!("Connection from {}", ip);

            let message = Message::text("{\"code\": \"Hello\"}");
            client.send_message(&message).unwrap();

            let (mut receiver, mut sender) = client.split().unwrap();

            let handle = thread::spawn(move || {
                for message in receiver.incoming_messages() {
                    let message = message.unwrap();
                    match &message {
                        OwnedMessage::Close(_) => {
                            let message = Message::close();
                            sender.send_message(&message).unwrap();
                            println!("Client {} disconnected", ip);
                            return;
                        }
                        OwnedMessage::Ping(data) => {
                            let message = Message::pong(data.to_owned());
                            sender.send_message(&message).unwrap();
                        }
                        OwnedMessage::Text(data) => {
                            println!("New message {}", data.to_string());
                        }
                        _ => {
                            sender.send_message(&message).unwrap();
                        }
                    }
                }
            });

            handle.join().unwrap();
        });
    }
}
