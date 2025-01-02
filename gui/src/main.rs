extern crate libc;

mod bindings;
mod link;
mod search;

use eframe::egui;
use link::draw_links;
use search::{perform_search, SearchBar};
use serde_json::{Map, Value};

pub struct MyApp {
    search_bar: SearchBar,
    results: Map<String, Value>,
    status_message: String,
    current_page: usize,   
    items_per_page: usize,  
}

impl Default for MyApp {
    fn default() -> Self {
        Self {
            search_bar: Default::default(),
            results: Map::new(),
            status_message: "Digite uma query e clique em buscar.".to_string(),
            current_page: 0,
            items_per_page: 10, 
        }
    }
}

impl eframe::App for MyApp {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        ctx.set_pixels_per_point(1.5);

        egui::CentralPanel::default().show(ctx, |ui| {
            egui::Frame::none()
                .inner_margin(crate::egui::Margin::symmetric(20.0, 0.0))
                .show(ui, |ui| {
                    ui.heading("Search bar implementada com TF-IDF");

                    if self.search_bar.ui(ui) {
                        let start_time = std::time::Instant::now();
                        
                        match perform_search(&self.search_bar.query) {
                            Ok(res) => {
                                self.results = res;
                                let elapsed = start_time.elapsed();
                                let secs = elapsed.as_secs_f64();
                                self.status_message = format!(
                                    "Exibindo {} resultados em {:.6?}s",
                                    self.results.len(),
                                    secs
                                );
                            }
                            Err(e) => {
                                self.results.clear();
                                let elapsed = start_time.elapsed();
                                let secs = elapsed.as_secs_f64();
                                self.status_message = format!("Erro após {:.6?}s: {}", secs, e);
                            }
                        }
                    }

                    ui.separator();
                    ui.label(&self.status_message);
                    ui.separator();

                    if !self.results.is_empty() {
                        let all_results: Vec<(String, Value)> = self
                            .results
                            .iter()
                            .map(|(k, v)| (k.clone(), v.clone()))
                            .collect();
                        let total_results = all_results.len();

                        let start_idx = self.current_page * self.items_per_page;
                        let end_idx = (start_idx + self.items_per_page).min(total_results);

                        if start_idx < total_results {
                            let page_slice = &all_results[start_idx..end_idx];

                            let mut page_map = Map::new();
                            for (title, val) in page_slice {
                                page_map.insert(title.clone(), val.clone());
                            }

                            draw_links(ui, &page_map);

                            ui.horizontal(|ui| {
                                if ui.button("Prev").clicked() {
                                    if self.current_page > 0 {
                                        self.current_page -= 1;
                                    }
                                }

                                let total_pages =
                                    (total_results + self.items_per_page - 1) / self.items_per_page;
                                ui.label(format!(
                                    "Página {} de {}",
                                    self.current_page + 1,
                                    total_pages
                                ));

                                if ui.button("Next").clicked() {
                                    if end_idx < total_results {
                                        self.current_page += 1;
                                    }
                                }
                            });
                        }
                    }
                });
        });
    }
}

fn main() -> eframe::Result<()> {
    env_logger::init();

    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default().with_maximized(true),
        ..Default::default()
    };

    eframe::run_native(
        "Search Engine",
        options,
        Box::new(|_cc| Ok(Box::new(MyApp::default()))),
    )
}
