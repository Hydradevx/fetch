use std::cmp::max;

use crate::core::{enviroment, hardware, system};
use crate::render::logo::{apply_logo_colors, visible_length};
use crate::util::{config::Config, distro};

pub fn display(config: &Config) {
    let out_lines = collect_modules(config);

    let logo_raw = distro::get_distro_logo(config);
    let logo_colors = vec![
        config.get_string("theme.color_primary", "#1793D1"),
        config.get_string("theme.color_secondary", "#0F3057"),
        config.get_string("theme.color_tertiary", "#00AEEF"),
    ];

    let logo_lines = apply_logo_colors(&logo_raw, &logo_colors);

    let max_logo_width = logo_lines
        .iter()
        .map(|l| visible_length(l))
        .max()
        .unwrap_or(0);

    let pad_left = config.get_int("general.padding_left", 0) as usize;
    let pad_right = config.get_int("general.padding_right", 0) as usize;

    let total_lines = max(logo_lines.len(), out_lines.len());

    for i in 0..total_lines {
        let logo_part = if i < logo_lines.len() {
            let mut line = logo_lines[i].clone();
            let pad = max_logo_width - visible_length(&line);
            line.push_str(&" ".repeat(pad));
            line
        } else {
            " ".repeat(max_logo_width)
        };

        let info_part = out_lines.get(i).map(String::as_str).unwrap_or("");

        println!(
            "{}{}{}{}",
            " ".repeat(pad_left),
            logo_part,
            " ".repeat(pad_right),
            info_part
        );
    }
}

fn collect_modules(config: &Config) -> Vec<String> {
    let mut out = Vec::new();

    for module in config.get_modules() {
        match module.as_str() {
            "os" => out.push(format!("OS: {}", system::get_os_name())),
            "host" => out.push(format!("Host: {}", system::get_host())),
            "kernel" => out.push(format!("Kernel: {}", system::get_kernel_version())),
            "uptime" => out.push(format!("Uptime: {}", system::get_uptime())),
            "cpu_model" => out.push(format!("CPU: {}", hardware::get_cpu_model())),
            "cpu_cores" => out.push(format!("CPU Cores: {}", hardware::get_cpu_core_count())),
            "gpu" => hardware::get_gpu_models()
                .into_iter()
                .for_each(|g| out.push(format!("GPU: {}", g))),
            "ram_total" => out.push(format!("Total RAM: {}", hardware::get_total_ram())),
            "ram_used" => out.push(format!("Used RAM: {}", hardware::get_used_ram())),
            "ram_available" => {
                out.push(format!("Available RAM: {}", hardware::get_available_ram()))
            }
            "swap_total" => out.push(format!("Total Swap: {}", hardware::get_total_swap())),
            "swap_free" => out.push(format!("Swap Free: {}", hardware::get_swap_free())),
            "disk_total" => out.push(format!("Disk Size: {}", hardware::get_disk_size())),
            "disk_used" => out.push(format!("Disk Used: {}", hardware::get_disk_used())),
            "resolution" => out.push(format!("Resolution: {}", hardware::get_resolution())),
            "shell" => out.push(format!("Shell: {}", enviroment::get_shell())),
            "terminal" => out.push(format!("Terminal: {}", enviroment::get_terminal())),
            "wm" => out.push(format!("WM: {}", enviroment::get_wm())),
            "packages" => out.push(format!("Packages: {}", enviroment::get_package_count())),
            "flatpaks" => out.push(format!("Flatpaks: {}", enviroment::get_flatpak_count())),
            "arch" => out.push(format!("Arch: {}", system::get_kernel_architecture())),
            _ => {}
        }
    }

    out
}
