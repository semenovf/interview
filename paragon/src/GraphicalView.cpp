#include <QLabel>
#include "Model.hpp"
#include "GraphicalView.hpp"

DiskSummaryWidget::DiskSummaryWidget (DiskModel & model, QWidget * parent)
    : QPushButton(parent)
    , _model(model)
{
    auto nameLabel = new QLabel(_model.name());
    auto typeLabel = new QLabel(toString(_model.type()));
    auto capacityLabel = new QLabel(toString(_model.capacity()));
}

GraphicalView::GraphicalView (QWidget * parent)
    : QWidget(parent)
{

}
