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
    // Channel for action
    let (tx, rx) = channel::<Action>();
    // Channel to bridge
    let tx_from_bridge = start_barcode_bridge(tx.clone());

    // Here we store the senders to message to WS write
    let mut action_subscribers: Vec<(String, Sender<Action>)> = Vec::new();

    // Main process
    let handle_main = thread::spawn(move || {
        for action in rx {
            match action {
                Action::Close(sender_key) => {
                    for sender in &action_subscribers {
                        // if sender.0 == sender_key {
                        //     // Send close event to the writer thread
                        //     sender.1.send(Action::Close(sender_key)).unwrap();
                        //     return;
                        // }
                    }
                    // Unsubscribe sender from the publish list
                    action_subscribers.retain(|x| x.0 != sender_key);
                }
                Action::Barcode(code) => {
                    for sender in &action_subscribers {
                        println!("Barcode action!");
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
            let client_id2 = client.peer_addr().unwrap().to_string();
            let (mut ws_rx, mut ws_tx) = client.split().unwrap();

            // Websocket read thread
            thread::spawn(move || {
                for message in ws_rx.incoming_messages() {
                    let message = message.unwrap();
                    match &message {
                        OwnedMessage::Close(_) => {
                            tx.send(Action::Close(client_id.clone())).unwrap();
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
                        Action::Close(_) => {
                            let message = Message::close();
                            ws_tx.send_message(&message).unwrap();
                            println!("Writer close");
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
