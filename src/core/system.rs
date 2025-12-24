use std::fs;

fn read_trim(path: &str) -> Option<String> {
    fs::read_to_string(path)
        .ok()
        .map(|s| s.trim().to_string())
        .filter(|s| !s.is_empty() && s != "Unknown")
}

pub fn get_host() -> String {
    if let Some(name) = read_trim("/sys/devices/virtual/dmi/id/product_name") {
        return name;
    }

    if let Some(board) = read_trim("/sys/devices/virtual/dmi/id/board_name") {
        return board;
    }

    if let Some(vendor) = read_trim("/sys/devices/virtual/dmi/id/sys_vendor") {
        return vendor;
    }

    "Unknown Device".to_string()
}

pub fn get_os_name() -> String {
    if let Some(os_name) = read_trim("/etc/os-release") {
        for line in os_name.lines() {
            if line.starts_with("PRETTY_NAME=") {
                let name = line
                    .trim_start_matches("PRETTY_NAME=")
                    .trim_matches('"')
                    .to_string();
                return name;
            }
        }
    }
    "Unknown OS".to_string()
}

pub fn get_kernel_version() -> String {
    if let Some(version) = read_trim("/proc/sys/kernel/osrelease") {
        return version;
    }
    "Unknown Kernel".to_string()
}

pub fn get_uptime() -> String {
    if let Some(uptime_str) = read_trim("/proc/uptime") {
        let parts: Vec<&str> = uptime_str.split_whitespace().collect();
        if let Some(uptime_seconds_str) = parts.get(0) {
            if let Ok(uptime_seconds) = uptime_seconds_str.parse::<f64>() {
                let hours = (uptime_seconds / 3600.0).floor() as u64;
                let minutes = ((uptime_seconds % 3600.0) / 60.0).floor() as u64;
                let seconds = (uptime_seconds % 60.0).floor() as u64;
                return format!("{}h {}m {}s", hours, minutes, seconds);
            }
        }
    }
    "Unknown Uptime".to_string()
}

pub fn get_kernel_architecture() -> String {
    let uname_output = std::process::Command::new("uname")
        .arg("-m")
        .output()
        .unwrap();
    let architecture = String::from_utf8_lossy(&uname_output.stdout)
        .trim()
        .to_string();
    if architecture.is_empty() {
        "Unknown Architecture".to_string()
    } else {
        architecture
    }
}
