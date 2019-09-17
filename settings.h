#pragma once

#include <QObject>

namespace Minimap::Internal {

class OptionsPage;

class Settings final : public QObject {
    Q_OBJECT

public:
    explicit Settings(QObject *parent) noexcept;
    ~Settings() noexcept;

    // For Utils::toSettings
    void toMap(const QString &prefix, QVariantMap *map) const noexcept;

    // For Utils::fromSettings
    void fromMap(const QString &prefix, const QVariantMap &map) noexcept;

    static Settings *instance() noexcept;

    static bool enabled() noexcept;
    static int width() noexcept;
    static int lineCountThreshold() noexcept;
    static int alpha() noexcept;

signals:
    void enabledChanged(bool);
    void widthChanged(int);
    void lineCountThresholdChanged(int);
    void alphaChanged(int);

private:
    friend class OptionsPage;

    void setEnabled(bool enabled) noexcept;
    void setWidth(int width) noexcept;
    void setLineCountThreshold(int lineCountThreshold) noexcept;
    void setAlpha(int alpha) noexcept;

    bool m_enabled = false;
    int m_width = 0;
    int m_lineCountThreshold = 0;
    int m_alpha = 0;
    OptionsPage *m_settingsPage = nullptr;
};

} // namespace Minimap::Internal
