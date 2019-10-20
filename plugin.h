#pragma once

#include <extensionsystem/iplugin.h>

class QStyle;

namespace Core {

class IEditor;

}

namespace Minimap::Internal {

class MinimapPlugin final : public ExtensionSystem::IPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE
                          "minimap.json")

public:
    explicit MinimapPlugin() noexcept;

    bool initialize(const QStringList &arguments,
                    QString *errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private:
    void editorCreated(Core::IEditor *editor, const QString &fileName) const
        noexcept;
    QStyle *m_oldStyle = nullptr;
};

} // namespace Minimap::Internal
