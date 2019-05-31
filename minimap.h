#pragma once

#include <extensionsystem/iplugin.h>

class QStyle;

namespace Core {

class IEditor;

}

namespace Minimap::Internal {

class MinimapPlugin : public ExtensionSystem::IPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE
                          "minimap.json")

public:
    MinimapPlugin() = default;
    ~MinimapPlugin() override = default;

    bool initialize(const QStringList &arguments,
                    QString *errorString) override;
    void extensionsInitialized() override {}

private:
    void editorCreated(Core::IEditor *editor, const QString &fileName);
};

} // namespace Minimap::Internal
