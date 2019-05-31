#pragma once

#include <QProxyStyle>

namespace TextEditor {

class BaseTextEditor;

}

namespace Minimap::Internal {

class MinimapStyleObject;

class MinimapStyle : public QProxyStyle {
    Q_OBJECT

public:
    MinimapStyle(QStyle *style);

    void drawComplexControl(ComplexControl control,
                            const QStyleOptionComplex *option,
                            QPainter *painter,
                            const QWidget *widget = Q_NULLPTR) const override;

    // need this due to QTBUG-24279
    SubControl
    hitTestComplexControl(ComplexControl control,
                          const QStyleOptionComplex *option,
                          const QPoint &pos,
                          const QWidget *widget = Q_NULLPTR) const override;

    int pixelMetric(PixelMetric metric,
                    const QStyleOption *option = Q_NULLPTR,
                    const QWidget *widget = Q_NULLPTR) const override;

    QRect subControlRect(ComplexControl cc,
                         const QStyleOptionComplex *opt,
                         SubControl sc,
                         const QWidget *widget) const override;

    static QObject *
    createMinimapStyleObject(TextEditor::BaseTextEditor *editor);

private:
    bool drawMinimap(const QStyleOptionComplex *,
                     QPainter *,
                     const QWidget *,
                     MinimapStyleObject *) const;
};

} // namespace Minimap::Internal
