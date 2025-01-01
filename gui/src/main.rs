extern crate libc;

mod bindings; 
mod search;   
mod link;    

use eframe::egui;
use search::{perform_search, SearchBar};
use link::draw_links;

pub struct MyApp {
    search_bar: SearchBar,
    results: Vec<(String, f64)>,
    status_message: String,
}

impl Default for MyApp {
    fn default() -> Self {
        Self {
            search_bar: Default::default(),
            results: Vec::new(),
            status_message: "Digite uma query e clique em buscar.".to_string(),
        }
    }
}

impl eframe::App for MyApp {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        egui::CentralPanel::default().show(ctx, |ui| {
            ui.heading("Search bar implementada com TF-IDF");

            if self.search_bar.ui(ui) {
                match perform_search(&self.search_bar.query) {
                    Ok(res) => {
                        self.results = res;
                        self.status_message = format!("Exibindo {} resultados.", self.results.len());
                    }
                    Err(e) => {
                        self.results.clear();
                        self.status_message = e;
                    }
                }
            }

            ui.separator();
            ui.label(&self.status_message);
            ui.separator();

            if !self.results.is_empty() {
                draw_links(ui, &self.results);
            }
        });
    }
}

fn main() -> eframe::Result<()> {
    env_logger::init();

    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default().with_inner_size([400.0, 300.0]),
        ..Default::default()
    };

    eframe::run_native(
        "Search Engine",
        options,
        Box::new(|_cc| Ok(Box::new(MyApp::default()))),
    )
}
