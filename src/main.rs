mod core;
mod util;
use core::{enviroment, hardware, system};
use std::cmp::max;
use util::{config::Config, distro};

pub fn display_register(config: &Config) -> Vec<String> {
    let mut out_lines: Vec<String> = Vec::new();

    for module in config.get_modules() {
        match module.as_str() {
            "os" => {
                out_lines.push(format!("OS: {}", system::get_os_name()));
            }

            "host" => {
                out_lines.push(format!("Host: {}", system::get_host()));
            }

            "kernel" => {
                out_lines.push(format!("Kernel: {}", system::get_kernel_version()));
            }

            "uptime" => {
                out_lines.push(format!("Uptime: {}", system::get_uptime()));
            }

            "cpu_model" => {
                out_lines.push(format!("CPU: {}", hardware::get_cpu_model()));
            }

            "cpu_cores" => {
                out_lines.push(format!("CPU Cores: {}", hardware::get_cpu_core_count()));
            }

            "gpu" => {
                for gpu in hardware::get_gpu_models() {
                    out_lines.push(format!("GPU: {}", gpu));
                }
            }

            "ram_total" => {
                out_lines.push(format!("Total RAM: {}", hardware::get_total_ram()));
            }

            "ram_used" => {
                out_lines.push(format!("Used RAM: {}", hardware::get_used_ram()));
            }

            "ram_available" => {
                out_lines.push(format!("Available RAM: {}", hardware::get_available_ram()));
            }

            "swap_total" => {
                out_lines.push(format!("Total Swap: {}", hardware::get_total_swap()));
            }

            "swap_free" => {
                out_lines.push(format!("Swap Free: {}", hardware::get_swap_free()));
            }

            "disk_total" => {
                out_lines.push(format!("Disk Size: {}", hardware::get_disk_size()));
            }

            "disk_used" => {
                out_lines.push(format!("Disk Used: {}", hardware::get_disk_used()));
            }

            "resolution" => {
                out_lines.push(format!("Resolution: {}", hardware::get_resolution()));
            }

            "shell" => {
                out_lines.push(format!("Shell: {}", enviroment::get_shell()));
            }

            "terminal" => {
                out_lines.push(format!("Terminal: {}", enviroment::get_terminal()));
            }

            "wm" => {
                out_lines.push(format!("WM: {}", enviroment::get_wm()));
            }

            "packages" => {
                out_lines.push(format!("Packages: {}", enviroment::get_package_count()));
            }

            "flatpaks" => {
                out_lines.push(format!("Flatpaks: {}", enviroment::get_flatpak_count()));
            }
            "arch" => {
                out_lines.push(format!("Arch: {}", system::get_kernel_architecture()));
            }
            _ => {}
        }
    }

    out_lines
}

fn ansi_fg_hex(hex: &str) -> String {
    let hex = hex.trim_start_matches('#');

    if hex.len() != 6 {
        return String::new();
    }

    let r = u8::from_str_radix(&hex[0..2], 16).unwrap_or(255);
    let g = u8::from_str_radix(&hex[2..4], 16).unwrap_or(255);
    let b = u8::from_str_radix(&hex[4..6], 16).unwrap_or(255);

    format!("\x1b[38;2;{};{};{}m", r, g, b)
}

fn apply_logo_colors(logo: &[String], colors: &[String]) -> Vec<String> {
    let mut result = Vec::with_capacity(logo.len());

    for line in logo {
        let mut out = line.clone();

        for (i, color) in colors.iter().enumerate() {
            let token = format!("${}", i + 1);
            let ansi = ansi_fg_hex(color);

            while let Some(pos) = out.find(&token) {
                out.replace_range(pos..pos + token.len(), &ansi);
            }
        }

        out.push_str("\x1b[0m"); // reset per line
        result.push(out);
    }

    result
}

fn visible_length(s: &str) -> usize {
    let mut len = 0;
    let mut chars = s.chars().peekable();

    while let Some(c) = chars.next() {
        if c == '\x1b' {
            if chars.peek() == Some(&'[') {
                chars.next(); // skip '['
                while let Some(ch) = chars.next() {
                    if ch.is_ascii_alphabetic() {
                        break;
                    }
                }
            }
        } else {
            len += 1;
        }
    }

    len
}

fn display(config: &Config) {
    let out_lines = display_register(config);

    let logo_raw = distro::get_distro_logo(config);

    let logo_colors = vec![
        config.get_string("theme.color_primary", "#1793D1"),
        config.get_string("theme.color_secondary", "#0F3057"),
        config.get_string("theme.color_tertiary", "#00AEEF"),
    ];

    let logo_lines = apply_logo_colors(&logo_raw, &logo_colors);

    let mut max_logo_width = 0usize;
    for line in &logo_lines {
        let len = visible_length(line);
        if len > max_logo_width {
            max_logo_width = len;
        }
    }

    let pad_left = config.get_int("general.padding_left", 0) as usize;
    let pad_right = config.get_int("general.padding_right", 0) as usize;

    let total_lines = max(logo_lines.len(), out_lines.len());

    for i in 0..total_lines {
        let mut logo_part = if i < logo_lines.len() {
            let mut line = logo_lines[i].clone();
            let pad = max_logo_width.saturating_sub(visible_length(&line));
            line.push_str(&" ".repeat(pad));
            line
        } else {
            " ".repeat(max_logo_width)
        };

        let info_part = if i < out_lines.len() {
            &out_lines[i]
        } else {
            ""
        };

        println!(
            "{}{}{}{}",
            " ".repeat(pad_left),
            logo_part,
            " ".repeat(pad_right),
            info_part
        );
    }
}

fn main() {
    let mut config = Config::new();

    if !config.load() {
        eprintln!("Failed to load configuration.");
        return;
    }

    display(&config);
}
