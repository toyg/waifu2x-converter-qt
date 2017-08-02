#include "processmodemodel.h"

ProcessModeModel::ProcessModeModel()
{

    QStandardItem *noise = new QStandardItem(tr("Noise Reduction"));
    QStandardItem *scale = new QStandardItem(tr("Upscale"));
    QStandardItem *noiseScale = new QStandardItem(tr("Upscale & Noise Reduction"));

    noise->setData("noise", Qt::UserRole);
    scale->setData("scale", Qt::UserRole);
    noiseScale->setData("noise_scale", Qt::UserRole);

    this->setItem(0, scale);
    this->setItem(1, noise);
    this->setItem(2, noiseScale);

}
