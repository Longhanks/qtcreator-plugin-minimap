#pragma once

#include <texteditor/displaysettings.h>
#include <texteditor/texteditoroptionspage.h>

#include <QCheckBox>
#include <QPointer>
#include <QSpinBox>

namespace Minimap::Internal {

class OptionsPage final : public TextEditor::TextEditorOptionsPage {

public:
    OptionsPage(QObject *parent, bool isTextWrappingEnabled) noexcept;
    ~OptionsPage() noexcept override = default;

    QWidget *widget() override;
    void apply() override;
    void finish() override;

private:
    void displaySettingsChanged(
        const TextEditor::DisplaySettings &settings) noexcept;

    QPointer<QWidget> m_widget = nullptr;
    QCheckBox *m_enabled = nullptr;
    QSpinBox *m_width = nullptr;
    QSpinBox *m_lineCountThresHold = nullptr;
    QSpinBox *m_alpha = nullptr;
    bool m_textWrapping;
};

} // namespace Minimap::Internal
