use std::fs;
use std::process::Command;

pub fn get_shell() -> String {
    if let Some(shell_path) = std::env::var_os("SHELL") {
        if let Some(shell_str) = shell_path.to_str() {
            let parts: Vec<&str> = shell_str.rsplit('/').collect();
            if let Some(shell_name) = parts.get(0) {
                return shell_name.to_string();
            }
        }
    }
    "Unknown Shell".to_string()
}

pub fn get_terminal() -> String {
    if let Some(term) = std::env::var_os("TERM") {
        if let Some(term_str) = term.to_str() {
            return term_str.to_string();
        }
    }
    "Unknown Terminal".to_string()
}

pub fn get_wm() -> String {
    if let Some(wm) = std::env::var_os("XDG_CURRENT_DESKTOP") {
        if let Some(wm_str) = wm.to_str() {
            return wm_str.to_string();
        }
    }
    "Unknown WM".to_string()
}

fn exists(path: &str) -> bool {
    fs::metadata(path).is_ok()
}

fn exec_and_trim(cmd: &str) -> Option<String> {
    let output = Command::new("sh").arg("-c").arg(cmd).output().ok()?;

    let s = String::from_utf8_lossy(&output.stdout);
    let trimmed = s.trim();

    if trimmed.is_empty() {
        None
    } else {
        Some(trimmed.to_string())
    }
}

pub fn get_package_count() -> String {
    if exists("/bin/pacman") || exists("/usr/bin/pacman") {
        if let Some(v) = exec_and_trim("pacman -Qq | wc -l") {
            return v;
        }
    }

    if exists("/usr/bin/apt") {
        if let Some(v) = exec_and_trim("dpkg -l | grep '^ii' | wc -l") {
            return v;
        }
    }

    if exists("/usr/bin/dnf") {
        if let Some(v) = exec_and_trim("dnf list installed | wc -l") {
            return v;
        }
    }

    if exists("/usr/bin/rpm") {
        if let Some(v) = exec_and_trim("rpm -qa | wc -l") {
            return v;
        }
    }

    if exists("/usr/bin/xbps-query") {
        if let Some(v) = exec_and_trim("xbps-query -l | wc -l") {
            return v;
        }
    }

    if exists("/usr/bin/emerge") {
        if let Some(v) = exec_and_trim("qlist -I | wc -l") {
            return v;
        }
    }

    if exists("/sbin/apk") {
        if let Some(v) = exec_and_trim("apk info | wc -l") {
            return v;
        }
    }

    if exists("/usr/bin/eopkg") {
        if let Some(v) = exec_and_trim("eopkg list-installed | wc -l") {
            return v;
        }
    }

    "Unknown".to_string()
}

pub fn get_flatpak_count() -> String {
    if exists("/usr/bin/flatpak") {
        if let Some(v) = exec_and_trim("flatpak list | wc -l") {
            return v;
        }
    }

    "0".to_string()
}
