mod core;
mod render;
mod util;

use util::config::Config;

fn main() {
    let mut config = Config::new();

    if !config.load() {
        eprintln!("Failed to load configuration.");
        return;
    }

    render::display::display(&config);
}
