use std::io::{BufRead, BufReader};
use std::process::{Command, Stdio};
use std::sync::mpsc::{channel, Sender, TryRecvError};
use std::thread;
use std::thread::sleep;
use std::time::Duration;

fn start_listener(sender: Sender<String>) {
    let cmd = std::env::current_dir().unwrap().join("scanner");
    let child = Command::new(cmd)
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .spawn()
        .expect("Failed to start ping process");

    println!("Started process: {}", child.id());

    thread::spawn(move || {
        let mut f = BufReader::new(child.stdout.unwrap());
        loop {
            let mut buf = String::new();
            match f.read_line(&mut buf) {
                Ok(_) => {
                    sender.send(buf).unwrap();
                }
                Err(e) => println!("an error!: {:?}", e),
            }
        }
    });
}

fn main() {
    let (tx, rx) = channel();
    start_listener(tx);

    loop {
        match rx.try_recv() {
            Ok(line) => {
                // Ok response from the scanner
                println!("{}", line);
            }
            Err(TryRecvError::Empty) => {
                // sleep(Duration::from_secs(1)); // ??
                continue;
            }
            Err(e) => {
                panic!(e);
            }
        }
    }
}
