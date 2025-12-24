pub fn ansi_fg_hex(hex: &str) -> String {
    let hex = hex.trim_start_matches('#');
    if hex.len() != 6 {
        return String::new();
    }

    let r = u8::from_str_radix(&hex[0..2], 16).unwrap_or(255);
    let g = u8::from_str_radix(&hex[2..4], 16).unwrap_or(255);
    let b = u8::from_str_radix(&hex[4..6], 16).unwrap_or(255);

    format!("\x1b[38;2;{};{};{}m", r, g, b)
}

pub fn apply_logo_colors(logo: &[String], colors: &[String]) -> Vec<String> {
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

        out.push_str("\x1b[0m");
        result.push(out);
    }

    result
}

pub fn visible_length(s: &str) -> usize {
    let mut len = 0;
    let mut chars = s.chars().peekable();

    while let Some(c) = chars.next() {
        if c == '\x1b' && chars.peek() == Some(&'[') {
            chars.next();
            while let Some(ch) = chars.next() {
                if ch.is_ascii_alphabetic() {
                    break;
                }
            }
        } else {
            len += 1;
        }
    }

    len
}
