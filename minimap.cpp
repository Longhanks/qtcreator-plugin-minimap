#include "minimap.h"

#include "minimapsettings.h"
#include "minimapstyle.h"

#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editormanager/ieditor.h>
#include <texteditor/texteditor.h>

#include <QApplication>

namespace Minimap::Internal {

bool MinimapPlugin::initialize([[maybe_unused]] const QStringList &arguments,
                               [[maybe_unused]] QString *errorMessage) {
    new MinimapSettings(this);

    auto minimapStyle = new MinimapStyle(QApplication::style());
    QApplication::setStyle(minimapStyle);
    QApplication::style()->moveToThread(QApplication::instance()->thread());

    Core::EditorManager *em = Core::EditorManager::instance();
    connect(em,
            &Core::EditorManager::editorCreated,
            this,
            &MinimapPlugin::editorCreated);

    return true;
}

void MinimapPlugin::editorCreated(Core::IEditor *editor,
                                  [[maybe_unused]] const QString &fileName) {
    auto baseEditor = qobject_cast<TextEditor::BaseTextEditor *>(editor);
    if (baseEditor != nullptr) {
        MinimapStyle::createMinimapStyleObject(baseEditor);
    }
}

} // namespace Minimap::Internal
