use std::io::{BufRead, BufReader};
use std::process::{Command, Stdio};

fn start_listener<T: 'static + Send + Fn(&str)>(cb: T) {
    let p = std::env::current_dir().unwrap().join("scanner");
    let child = Command::new(p)
        .stdout(Stdio::piped())
        .spawn()
        .expect("Failed to start ping process");
    println!("Started process: {}", child.id());
    let mut f = BufReader::new(child.stdout.unwrap());
    let mut buf = String::new();
    loop {
        match f.read_line(&mut buf) {
            Ok(_) => {
                cb(buf.as_str());
            }
            Err(e) => println!("an error!: {:?}", e),
        }
    }
}

fn main() {
    start_listener(|s| {
        println!("Got this back: {}", s);
    });
}
