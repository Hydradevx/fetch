

pub fn get_cpu_model() -> String {
  let cpuinfo = std::fs::read_to_string("/proc/cpuinfo").unwrap();
  for line in cpuinfo.lines() {
    if line.starts_with("model name") {
      let parts: Vec<&str> = line.split(':').collect();
      if parts.len() > 1 {
        return parts[1].trim().to_string();
      }
    }
  }
  "Unknown CPU".to_string()
}

pub fn get_cpu_core_count() -> usize {
  let cpuinfo = std::fs::read_to_string("/proc/cpuinfo").unwrap();
  let mut count = 0;
  for line in cpuinfo.lines() {
    if line.starts_with("processor") {
      count += 1;
    }
  }
  count
}

pub fn get_gpu_models() -> Vec<String> {
  let lspci_output = std::process::Command::new("lspci")
    .arg(" | grep -Ei 'vga|3d|display'")
    .output()
    .expect("Failed to execute lspci command");
  let output_str = String::from_utf8_lossy(&lspci_output.stdout);
  let mut gpus = Vec::new();
  for line in output_str.lines() {
    if line.contains("VGA compatible controller") || line.contains("3D controller") {
      let parts: Vec<&str> = line.split('"').collect();
      if parts.len() > 3 {
        gpus.push(parts[3].to_string());
      }
    }
  }
  gpus
}

pub fn get_total_ram() -> String {
  let meminfo = std::fs::read_to_string("/proc/meminfo").unwrap();
  for line in meminfo.lines() {
    if line.starts_with("MemTotal:") {
      let parts: Vec<&str> = line.split_whitespace().collect();
      if parts.len() > 1 {
        if let Ok(kb) = parts[1].parse::<u64>() {
          return (kb / 1024).to_string() + " MB"; // Convert to MB
        }
      }
    }
  }
  "Unknown RAM".to_string()
}

pub fn get_available_ram() -> String {
  let meminfo = std::fs::read_to_string("/proc/meminfo").unwrap();
  for line in meminfo.lines() {
    if line.starts_with("MemAvailable:") {
      let parts: Vec<&str> = line.split_whitespace().collect();
      if parts.len() > 1 {
        if let Ok(kb) = parts[1].parse::<u64>() {
          return (kb / 1024).to_string() + " MB"; // Convert to MB
        }
      }
    }
  }
  "Unknown Available RAM".to_string()
}

pub fn get_used_ram() -> String {
  let total_ram_str = get_total_ram();
  let available_ram_str = get_available_ram();

  let total_ram_mb: u64 = total_ram_str
    .split_whitespace()
    .next()
    .unwrap_or("0")
    .parse()
    .unwrap_or(0);
  let available_ram_mb: u64 = available_ram_str
    .split_whitespace()
    .next()
    .unwrap_or("0")
    .parse()
    .unwrap_or(0);

  if total_ram_mb >= available_ram_mb {
    let used_ram_mb = total_ram_mb - available_ram_mb;
    return used_ram_mb.to_string() + " MB";
  }

  "Unknown Used RAM".to_string()
}

pub fn get_total_swap() -> String {
  let meminfo = std::fs::read_to_string("/proc/meminfo").unwrap();
  for line in meminfo.lines() {
    if line.starts_with("SwapTotal:") {
      let parts: Vec<&str> = line.split_whitespace().collect();
      if parts.len() > 1 {
        if let Ok(kb) = parts[1].parse::<u64>() {
          return (kb / 1024).to_string() + " MB"; // Convert to MB
        }
      }
    }
  }
  "Unknown Swap".to_string()
}

pub fn get_swap_free() -> String {
  let meminfo = std::fs::read_to_string("/proc/meminfo").unwrap();
  for line in meminfo.lines() {
    if line.starts_with("SwapFree:") {
      let parts: Vec<&str> = line.split_whitespace().collect();
      if parts.len() > 1 {
        if let Ok(kb) = parts[1].parse::<u64>() {
          return (kb / 1024).to_string() + " MB"; // Convert to MB
        }
      }
    }
  }
  "Unknown Swap Free".to_string()
}

pub fn get_disk_size() -> String {
    let output = std::process::Command::new("df")
        .arg("-h")
        .output()
        .expect("Failed to execute df");

    let output_str = String::from_utf8_lossy(&output.stdout);

    if let Some(line) = output_str.lines().nth(1) {
        let parts: Vec<&str> = line.split_whitespace().collect();
        if parts.len() > 1 {
            return parts[1].to_string();
        }
    }

    "Unknown Disk Size".to_string()
}

pub fn get_disk_used() -> String {
    let output = std::process::Command::new("df")
        .arg("-h")
        .output()
        .expect("Failed to execute df");

    let output_str = String::from_utf8_lossy(&output.stdout);

    if let Some(line) = output_str.lines().nth(1) {
        let parts: Vec<&str> = line.split_whitespace().collect();
        if parts.len() > 2 {
            return parts[2].to_string();
        }
    }

    "Unknown Disk Used".to_string()
}

pub fn get_resolution() -> String {
    let output = std::process::Command::new("xdpyinfo")
        .output()
        .expect("Failed to execute xdpyinfo");

    let output_str = String::from_utf8_lossy(&output.stdout);

    for line in output_str.lines() {
        if line.contains("dimensions:") {
            let parts: Vec<&str> = line.split_whitespace().collect();
            if parts.len() >= 2 {
                return parts[1].to_string();
            }
        }
    }

    "Unknown Resolution".to_string()
}
