use eframe::egui::{Ui, RichText, Label, Color32};
use serde_json::{Map, Value};

pub fn draw_links(ui: &mut Ui, results: &Map<String, Value>) {
    for (title, data) in results {
        let summary = data["summary"].as_str().unwrap_or("");

        let truncated_summary = if summary.len() > 195 {
            format!("{}...", &summary[..195])
        } else {
            summary.to_string()
        };

        ui.add(
            Label::new(
                RichText::new(title)
                    .heading()    
                    .underline()      
                    .color(Color32::from_rgb(153, 195, 255)), 
            )
        );

        ui.add_space(4.0);

        ui.label(
            RichText::new(truncated_summary)
                .small()
        );

        ui.add_space(12.0);
    }
}
