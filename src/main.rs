use std::io::Write;
use std::io::{BufRead, BufReader};
use std::process::Child;
use std::process::{ChildStdin, ChildStdout};
use std::process::{Command, Stdio};
use std::sync::mpsc::Receiver;
use std::sync::mpsc::{channel, Sender, TryRecvError};
use std::thread;
use std::thread::JoinHandle;

fn start_listener(stdout: ChildStdout, sender: Sender<String>, receiver: Receiver<String>) {
    let mut f = BufReader::new(stdout);
    thread::spawn(move || loop {
        let mut buf = String::new();
        match f.read_line(&mut buf) {
            Ok(_) => {
                // stdin.write_all(b"2\n").unwrap();
                sender.send(buf).unwrap();
            }
            Err(e) => println!("an error!: {:?}", e),
        }
    });
}

fn read_process(rx_stdout: Receiver<String>) -> JoinHandle<()> {
    thread::spawn(move || {
        for line in rx_stdout {
            println!("{}", getcode(&line).unwrap());
        }
    })
}

fn getcode(from: &str) -> Option<String> {
    if let Some(start) = from.find("<datalabel>") {
        if let Some(end) = from.find("</datalabel>") {
            if start > 0 && end < from.len() {
                let raw = from[start + 11..end].trim().to_string();
                let raw_array = raw
                    .split(" ")
                    .into_iter()
                    .map(|c| u8::from_str_radix(c, 16).unwrap())
                    .collect::<Vec<u8>>();
                if let Ok(string) = std::str::from_utf8(&raw_array) {
                    return Some(string.to_string());
                }
            }
        }
    }
    None
}

fn main() {
    let cmd = std::env::current_dir().unwrap().join("scanner");
    let mut child = Command::new(cmd)
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .spawn()
        .expect("Failed to start ping process");
    let (tx_stdout, rx_stdout) = channel::<String>();
    let (tx_stdin, rx_stdin) = channel::<String>();
    start_listener(child.stdout.unwrap(), tx_stdout, rx_stdin);
    let _ = read_process(rx_stdout);

    let mut input = String::new();
    let mut stdin = child.stdin.take().unwrap();
    loop {
        match std::io::stdin().read_line(&mut input) {
            Ok(_) => {
                stdin.write_all(input.as_bytes()).unwrap();
                input = "".to_string();
            }
            Err(error) => println!("error: {}", error),
        }
    }

    // read_process.join().unwrap();
}
