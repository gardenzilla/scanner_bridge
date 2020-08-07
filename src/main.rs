extern crate hyper;
extern crate websocket;

// use hyper::Fresh;
use std::sync::mpsc::channel;
use std::sync::mpsc::Sender;
use std::thread;
use websocket::sync::Server;
use websocket::{Message, OwnedMessage};

mod barcode_bridge;
use barcode_bridge::*;

fn main() {
    // Bind websocket server
    let ws_server = Server::bind("127.0.0.1:2794").unwrap();
    // Channel for ..
    let (tx, rx) = channel::<Action>();
    // Channel for ..
    // let (tx2, rx2) = channel::<Action>();
    let tx_for_bridge = tx.clone();
    let tx_from_bridge = start_barcode_bridge(tx_for_bridge);

    // Here we store the senders to message to WS write
    let mut action_subscribers: Vec<(String, Sender<Action>)> = Vec::new();

    // Main process
    let handle_main = thread::spawn(move || {
        for action in rx {
            match action {
                Action::Close => {
                    for sender in &action_subscribers {
                        match sender.1.send(Action::Close) {
                            Ok(_) => (),
                            Err(_) => (),
                        }
                    }
                    return;
                }
                Action::Barcode(code) => {
                    for sender in &action_subscribers {
                        match sender.1.send(Action::Barcode(code.to_owned())) {
                            Ok(_) => (),
                            Err(_) => (),
                        }
                    }
                }
                Action::Error => tx_from_bridge.send(BridgeAction::Error).unwrap(),
                Action::Subscribe(id, sender) => {
                    action_subscribers.push((id, sender));
                    // action_subscribers
                    //     .last()
                    //     .unwrap()
                    //     .send(Action::SendSubsciberKey(action_subscribers.len() - 1))
                    //     .unwrap();
                }
                Action::Unsubscribe(id) => {
                    action_subscribers.retain(|x| x.0 != id);
                }
                _ => {
                    for sender in &action_subscribers {
                        match sender.1.send(Action::Other) {
                            Ok(_) => (),
                            Err(_) => (),
                        }
                    }
                }
            }
        }
    });
    // Start listening for WebSocket connections
    for connection in ws_server.filter_map(Result::ok) {
        // Spawn a new thread for each connection.
        let tx = tx.clone();
        // Main loop sender clone for WS communication
        let tx_action = tx.clone();
        thread::spawn(|| {
            if !connection
                .protocols()
                .contains(&"scannerbridge".to_string())
            {
                connection.reject().unwrap();
                return;
            }

            let client = connection.use_protocol("scannerbridge").accept().unwrap();
            let ip = client.peer_addr().unwrap();
            println!("Client IP is {}", ip);

            // Channel for websocket
            let client_id = client.peer_addr().unwrap().to_string();
            let client_id2 = client_id.clone();
            let (mut ws_rx, mut ws_tx) = client.split().unwrap();

            // Websocket read thread
            thread::spawn(move || {
                for message in ws_rx.incoming_messages() {
                    let message = message.unwrap();
                    match &message {
                        OwnedMessage::Close(_) => {
                            tx.send(Action::Close).unwrap();
                            println!("Socket closed {}", client_id);
                            return;
                        }
                        OwnedMessage::Ping(_) => {
                            tx.send(Action::Ping).unwrap();
                        }
                        OwnedMessage::Text(data) => {
                            if &data[..] == "\"error\"" {
                                tx.send(Action::Error).unwrap()
                            }
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
                // Lets create local channel
                let (ltx, lrx) = channel::<Action>();
                // Subscibe the local sender to the main loop
                tx_action.send(Action::Subscribe(client_id2, ltx)).unwrap();
                for action in lrx {
                    match action {
                        // Action::SendSubsciberKey(_index) => index = _index,
                        Action::Close => {
                            let message = Message::close();
                            ws_tx.send_message(&message).unwrap();
                            return;
                        }
                        Action::Barcode(code) => {
                            ws_tx
                                .send_message(&Message::text(format!("{{\"code\": \"{}\"}}", code)))
                                .unwrap();
                        }
                        _ => {}
                    }
                }
            });
        });
    }
    let _ = handle_main.join();
}
