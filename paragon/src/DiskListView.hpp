#pragma once
#include "TableView.hpp"

class Model;

class DiskListView : public TableView
{
    Model * _model;

public:
    DiskListView (Model *, QWidget * parent = 0);
};
