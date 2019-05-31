#pragma once

#include <QObject>

namespace Minimap::Internal {

class MinimapSettingsPage;

class MinimapSettings : public QObject {
    Q_OBJECT

public:
    explicit MinimapSettings(QObject *parent);
    ~MinimapSettings();

    void toMap(const QString &prefix, QVariantMap *map) const;
    void fromMap(const QString &prefix, const QVariantMap &map);

    static MinimapSettings *instance();

    static bool enabled();
    static int width();
    static int lineCountThreshold();
    static int alpha();

signals:
    void enabledChanged(bool);
    void widthChanged(int);
    void lineCountThresholdChanged(int);
    void alphaChanged(int);

private:
    friend class MinimapSettingsPage;

    void setEnabled(bool enabled);
    void setWidth(int width);
    void setLineCountThreshold(int lineCountThreshold);
    void setAlpha(int alpha);

    bool m_enabled;
    int m_width;
    int m_lineCountThreshold;
    int m_alpha;
    MinimapSettingsPage *m_settingsPage;
};

} // namespace Minimap::Internal
