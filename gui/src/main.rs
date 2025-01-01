extern crate libc;

mod bindings;
use bindings::*;

mod search;

use std::ffi::CString;
use std::fs::File;
use std::io::Read;

use eframe::egui;
use serde_json::{Map, Value};

fn main() -> eframe::Result {
    env_logger::init();
    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default().with_inner_size([320.0, 240.0]),
        ..Default::default()
    };
    eframe::run_native(
        "Search Engine",
        options,
        Box::new(|_cc| Ok(Box::new(MyApp::default()))),
    )
}

struct MyApp {
    input_text: String,
    result_text: String,
}

impl Default for MyApp {
    fn default() -> Self {
        Self {
            input_text: String::new(),
            result_text: String::from("Enter a query and click search."),
        }
    }
}

impl eframe::App for MyApp {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        egui::CentralPanel::default().show(ctx, |ui| {
            ui.heading("Search bar implemented based on TF-IDF algorithm");
            ui.horizontal(|ui| {
                let input_label = ui.label("Your search: ");
                ui.text_edit_singleline(&mut self.input_text)
                    .labelled_by(input_label.id);
            });

            if ui.button("Search").clicked() {
                let result = self.perform_search(&self.input_text);
                self.result_text = result;
            }

            ui.label("Results:");
            ui.label(&self.result_text);
        });
    }
}

impl MyApp {
    fn perform_search(&self, query: &str) -> String {
        if query.is_empty() {
            return "Query cannot be empty.".to_string();
        }

        unsafe {
            let mut engine: TFIDFEngine = std::mem::zeroed();
            let query_cstring = CString::new(query).expect("Failed to convert query to CString");

            tfidf_search(&mut engine, query_cstring.as_ptr());
        }

        match self.read_results() {
            Ok(results) => results,
            Err(err) => format!("Error reading results: {}", err),
        }
    }

    fn read_results(&self) -> Result<String, String> {
        let current_exe = std::env::current_exe().map_err(|e| format!("Exe path error: {}", e))?;
        let mut path = current_exe
            .parent()
            .ok_or("Failed to get parent directory")?
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

        let mut result_string = String::new();
        for (doc, score) in json_map.iter() {
            let score = score.as_f64().unwrap_or(0.0);
            result_string.push_str(&format!("{}: {:.6}\n", doc, score));
        }

        Ok(result_string)
    }
}