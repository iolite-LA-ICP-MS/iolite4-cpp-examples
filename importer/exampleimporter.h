#ifndef EXAMPLEIMPORTER_H
#define EXAMPLEIMPORTER_H

#include "/Applications/iolite4beta.app/Contents/include/abstractimporter.h"
#include "/Applications/iolite4beta.app/Contents/include/plugininterface.h"

using namespace IolitePlugin;

class ExampleImporter : public AbstractImporter, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.iolite-software.iolite4.PluginInterface")
    Q_INTERFACES(IolitePlugin::PluginInterface)

public:
    ExampleImporter();

    bool correctFormat() override;
    QObject *create() override { return new ExampleImporter(); }

public slots:
    void importData() override;
};

#endif // EXAMPLEIMPORTER_H
