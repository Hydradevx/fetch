use std::env;
use std::fs;
use std::fs::File;
use std::io::{Read, Write};
use std::path::PathBuf;

use serde_json::{Value, json};

static DEFAULT_CONFIG: &str = r##"
{
  "general": {
    "logo": "auto",
    "padding_left": 2,
    "padding_right": 2
  },
  "theme": {
    "color_primary": "#8aadf4",
    "color_secondary": "#c6d0f5"
  },
  "modules": [
    "os",
    "cpu",
    "gpu",
    "ram",
    "packages"
  ]
}
"##;

pub struct Config {
    root: Value,
}

impl Config {
    pub fn new() -> Self {
        Self {
            root: json!({}),
        }
    }

    pub fn get_config_path(&self) -> Option<PathBuf> {
        env::var("HOME")
            .ok()
            .map(|home| PathBuf::from(home).join(".config/hyprfetch/config.json"))
    }

    pub fn exists(&self) -> bool {
        self.get_config_path()
            .map(|p| p.exists())
            .unwrap_or(false)
    }

    pub fn load(&mut self) -> bool {
        let path = match self.get_config_path() {
            Some(p) => p,
            None => return false,
        };

        if let Some(parent) = path.parent() {
            if let Err(e) = fs::create_dir_all(parent) {
                eprintln!("Failed to create config directory: {}", e);
                return false;
            }
        }

        if !path.exists() {
            if let Ok(mut file) = File::create(&path) {
                let _ = file.write_all(DEFAULT_CONFIG.as_bytes());
                println!("Created default config at {}", path.display());
            }
        }

        let mut contents = String::new();
        match File::open(&path).and_then(|mut f| f.read_to_string(&mut contents)) {
            Ok(_) => match serde_json::from_str::<Value>(&contents) {
                Ok(json) => {
                    self.root = json;
                    true
                }
                Err(e) => {
                    eprintln!("Config parse error: {}", e);
                    false
                }
            },
            Err(e) => {
                eprintln!("Failed to read config: {}", e);
                false
            }
        }
    }

    fn resolve_path<'a>(&'a self, path: &str) -> Option<&'a Value> {
        let mut current = &self.root;

        for key in path.split('.') {
            match current.get(key) {
                Some(v) => current = v,
                None => return None,
            }
        }

        Some(current)
    }

    pub fn get_string(&self, path: &str, fallback: &str) -> String {
        self.resolve_path(path)
            .and_then(|v| v.as_str())
            .unwrap_or(fallback)
            .to_string()
    }

    pub fn get_int(&self, path: &str, fallback: i64) -> i64 {
        self.resolve_path(path)
            .and_then(|v| v.as_i64())
            .unwrap_or(fallback)
    }

    pub fn get_modules(&self) -> Vec<String> {
        match self.root.get("modules").and_then(|v| v.as_array()) {
            Some(arr) => arr
                .iter()
                .filter_map(|v| v.as_str().map(|s| s.to_string()))
                .collect(),
            None => Vec::new(),
        }
    }
}

pub static mut CONFIG: Option<Config> = None;
