use super::config::Config;
use std::env;
use std::fs;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::path::Path;

fn read_trim(path: &str) -> Option<String> {
    fs::read_to_string(path)
        .ok()
        .map(|s| s.trim().to_string())
        .filter(|s| !s.is_empty())
}

fn read_lines(path: &str) -> Option<Vec<String>> {
    fs::read_to_string(path)
        .ok()
        .map(|s| s.lines().map(|l| l.to_string()).collect())
}

pub fn get_logo_path(name: &str) -> String {
    if let Some(home) = env::var_os("HOME") {
        let path = Path::new(&home)
            .join(".config/hyprfetch/logos")
            .join(format!("{name}.txt"));

        if path.exists() {
            return path.to_string_lossy().to_string();
        }
    }

    String::new()
}

fn strip_quotes(s: &str) -> String {
    s.trim_matches('"').to_string()
}

pub fn get_distro_id() -> String {
    if let Some(lines) = read_lines("/etc/os-release") {
        for line in lines {
            if line.starts_with("ID=") {
                return strip_quotes(&line[3..]);
            }
        }
    }
    "linux".to_string()
}

pub fn get_distro_id_like() -> String {
    if let Some(lines) = read_lines("/etc/os-release") {
        for line in lines {
            if line.starts_with("ID_LIKE=") {
                let val = strip_quotes(&line[8..]);
                return val.split_whitespace().next().unwrap_or("").to_string();
            }
        }
    }
    String::new()
}

pub fn get_distro_logo(config: &Config) -> Vec<String> {
    let mut id = config.get_string("general.logo", "auto");

    if id == "auto" {
        id = get_distro_id();
    } else {
        id = strip_quotes(&id);
    }

    let mut path = get_logo_path(&id);

    if path.is_empty() {
        let like = get_distro_id_like();
        path = get_logo_path(&like);

        if path.is_empty() {
            path = get_logo_path("default");
        }
    }

    let file = File::open(&path);
    let mut lines = Vec::new();

    if let Ok(file) = file {
        for line in BufReader::new(file).lines().flatten() {
            lines.push(line);
        }
    }

    if lines.is_empty() {
        lines.push("  Love from Hydra  ".to_string());
    }

    lines
}
