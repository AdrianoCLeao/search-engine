use eframe::egui::{Label, RichText, Ui};

pub fn draw_links(ui: &mut Ui, results: &[(String, f64)]) {
    for (doc, score) in results {
        ui.horizontal(|ui| {
            ui.add(Label::new(
                RichText::new(doc).underline()
            ));
            ui.label(format!("{:.6}", score));
        });
    }
}
