#include "settings.h"

#include "constants.h"
#include "optionspage.h"

#include <coreplugin/icore.h>
#include <extensionsystem/pluginmanager.h>
#include <texteditor/texteditorsettings.h>
#include <utils/settingsutils.h>

#include <limits>

namespace Minimap::Internal {

static constexpr const char kEnabled[] = "Enabled";
static constexpr const char kWidth[] = "Width";
static constexpr const char kLineCountThreshold[] = "LineCountThresHold";
static constexpr const char kAlpha[] = "Alpha";

static Settings *m_instance = nullptr;

Settings::Settings(QObject *parent) noexcept
    : QObject(parent), m_enabled(true),
      m_width(Constants::MINIMAP_WIDTH_DEFAULT),
      m_lineCountThreshold(Constants::MINIMAP_MAX_LINE_COUNT_DEFAULT),
      m_alpha(Constants::MINIMAP_ALPHA_DEFAULT) {
    m_instance = this;
    Utils::fromSettings(QLatin1String("Minimap"),
                        QLatin1String("text"),
                        Core::ICore::settings(),
                        m_instance);
    this->m_settingsPage = new OptionsPage(
        this,
        TextEditor::TextEditorSettings::displaySettings().m_textWrapping);
    ExtensionSystem::PluginManager::addObject(this->m_settingsPage);
}

Settings::~Settings() noexcept {
    ExtensionSystem::PluginManager::removeObject(this->m_settingsPage);
    m_instance = nullptr;
}

Settings *Settings::instance() noexcept {
    return m_instance;
}

void Settings::toMap(const QString &prefix, QVariantMap *map) const noexcept {
    map->insert(prefix + QLatin1String(kEnabled), this->m_enabled);
    map->insert(prefix + QLatin1String(kWidth), this->m_width);
    map->insert(prefix + QLatin1String(kLineCountThreshold),
                this->m_lineCountThreshold);
    map->insert(prefix + QLatin1String(kAlpha), this->m_alpha);
}

void Settings::fromMap(const QString &prefix,
                       const QVariantMap &map) noexcept {
    this->m_enabled =
        map.value(prefix + QLatin1String(kEnabled), this->m_enabled).toBool();
    this->m_width =
        map.value(prefix + QLatin1String(kWidth), this->m_width).toInt();
    this->m_lineCountThreshold =
        map.value(prefix + QLatin1String(kLineCountThreshold),
                  this->m_lineCountThreshold)
            .toInt();
    this->m_alpha =
        map.value(prefix + QLatin1String(kAlpha), this->m_alpha).toInt();
}

bool Settings::enabled() noexcept {
    return m_instance->m_enabled;
}

int Settings::width() noexcept {
    return m_instance->m_width;
}

int Settings::lineCountThreshold() noexcept {
    return m_instance->m_lineCountThreshold;
}

int Settings::alpha() noexcept {
    return m_instance->m_alpha;
}

void Settings::setEnabled(bool enabled) noexcept {
    if (this->m_enabled != enabled) {
        this->m_enabled = enabled;
        emit enabledChanged(enabled);
    }
}

void Settings::setWidth(int width) noexcept {
    if (this->m_width != width) {
        this->m_width = width;
        emit widthChanged(width);
    }
}

void Settings::setLineCountThreshold(int lineCountThreshold) noexcept {
    if (this->m_lineCountThreshold != lineCountThreshold) {
        this->m_lineCountThreshold = lineCountThreshold;
        emit lineCountThresholdChanged(lineCountThreshold);
    }
}

void Settings::setAlpha(int alpha) noexcept {
    if (this->m_alpha != alpha) {
        this->m_alpha = alpha;
        emit alphaChanged(alpha);
    }
}

} // namespace Minimap::Internal
