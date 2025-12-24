mod core;
mod util;
use core::{hardware, system};
use util::config::Config;

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

            // "shell" => {
            //     out_lines.push(format!(
            //         "Shell: {}",
            //         environment::get_shell()
            //     ));
            // }

            // "terminal" => {
            //     out_lines.push(format!(
            //         "Terminal: {}",
            //         environment::get_terminal()
            //     ));
            // }

            // "wm" => {
            //     out_lines.push(format!(
            //         "WM: {}",
            //         system::get_wm()
            //     ));
            // }

            // "packages" => {
            //     out_lines.push(format!(
            //         "Packages: {}",
            //         system::get_package_count()
            //     ));
            // }

            // "flatpaks" => {
            //     out_lines.push(format!(
            //         "Flatpaks: {}",
            //         system::get_flatpak_count()
            //     ));
            // }
            "arch" => {
                out_lines.push(format!("Arch: {}", system::get_kernel_architecture()));
            }
            _ => {}
        }
    }

    out_lines
}

fn main() {
    let mut config = Config::new();

    if !config.load() {
        eprintln!("Failed to load configuration.");
        return;
    }
    let outlines = display_register(&config);
    for line in outlines {
        println!("{}", line);
    }
}
