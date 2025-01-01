use std::ffi::CString;
use std::fs::File;
use std::io::Read;
use std::mem;

use eframe::egui::{self, Frame, Margin, Rounding, Stroke, Ui};
use serde_json::{Map, Value};

use crate::bindings::{tfidf_search, TFIDFEngine};

#[derive(Default)]
pub struct SearchBar {
    pub query: String,
}

impl SearchBar {
    pub fn ui(&mut self, ui: &mut Ui) -> bool {
        let mut search_triggered = false;

        ui.horizontal(|ui| {
            Frame::none()
                .rounding(Rounding::same(10.0))
                .stroke(Stroke::new(1.0, ui.visuals().widgets.inactive.bg_stroke.color))
                .inner_margin(Margin::symmetric(8.0, 4.0))
                .show(ui, |ui| {
                    ui.add(
                        egui::TextEdit::singleline(&mut self.query)
                            .hint_text("Digite sua busca...")
                    );
                });
        
            if ui.button("🔎").clicked() {
            }
        });

        search_triggered
    }
}


pub fn perform_search(query: &str) -> Result<Vec<(String, f64)>, String> {
    if query.is_empty() {
        return Err("Query não pode ser vazia.".to_string());
    }

    unsafe {
        let mut engine: TFIDFEngine = mem::zeroed();
        let query_cstring = CString::new(query).map_err(|e| format!("Erro CString: {}", e))?;

        tfidf_search(&mut engine, query_cstring.as_ptr());
    }

    read_results()
}

fn read_results() -> Result<Vec<(String, f64)>, String> {
    let current_exe = std::env::current_exe().map_err(|e| format!("Exe path error: {}", e))?;
    let mut path = current_exe
        .parent()
        .ok_or("Falha ao obter o diretório pai")?
        .to_path_buf();

    path.pop();
    path.pop();
    path.pop();
    path.push("results.json");

    println!("Tentando abrir o arquivo em: {}", path.display());

    let mut file = File::open(&path).map_err(|e| format!("File error: {}", e))?;
    let mut contents = String::new();
    file.read_to_string(&mut contents)
        .map_err(|e| format!("Read error: {}", e))?;

    let json_map: Map<String, Value> =
        serde_json::from_str(&contents).map_err(|e| format!("Parse error: {}", e))?;

    let mut results = Vec::new();
    for (doc, score) in json_map {
        let score = score.as_f64().unwrap_or(0.0);
        results.push((doc, score));
    }

    Ok(results)
}
