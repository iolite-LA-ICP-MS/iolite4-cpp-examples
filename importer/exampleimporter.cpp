#include "exampleimporter.h"

#include <armadillo>

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QThread>

// Plugin constructor. This must initialize the plugin metadata.
ExampleImporter::ExampleImporter()
    : AbstractImporter(),
      PluginInterface({Importer,                // Type -- always Importer
                      "Example Importer",       // Name
                      "This is an example",     // Description
                      "1.0",                    // Version
                      "Joe Petrus",             // Authors
                      "support@iolite.xyz",     // Contact
                      "None"})                  // References
{
    // Optionally do some other initialization here...
    setProperty("task_title", "Example Importer");
}

/* This method will be called by iolite when the user selects a file
 * to import. Typically, it uses the provided name (stored in the class
 * as m_fileName) and parses as much of it as necessary to determine
 * if this importer is appropriate to import the data. For example,
 * although X Series II and Agilent data are both comma separated
 * value files, they can be distinguished by the characteristic
 * formatting in each. In our implementation, distinguishing the
 * two is done with 'regular expression' (QRegularExpression)
 * parsing of the first several lines of the file.
 *
 * Keep in mind that there is nothing stopping you from just
 * returning true (thus supporting all files!) or simply checking
 * the file extension, but such generic checks can yield unexpected
 * results. You cannot be sure which order the various importer
 * plugins will be checked for compatibility.
 */
bool ExampleImporter::correctFormat()
{
    // As an example, we'll accept all files ending with .ioe
    if (m_fileName.endsWith("ioe"))
        return true;

    return false;
}

/* This method uses the provided file name (stored in the class as
 * m_fileName), parses its contents, and registers time series data
 * with iolite by emitting the timeSeriesData signal.
 *
 * Importer progress can be updated via the 'message' and 'progress'
 * signals. These will be displayed in the iolite interface.
 *
 * When finished, the 'finished' signal should be emitted.
 */
void ExampleImporter::importData()
{
    // A 'task' signal is emitted to register the importer with iolite
    // so that progress and messages can be displayed during processing.
    emit task("Example Importer");

    // The file name selected by the user is stored in m_fileName.
    // Typically, one would access the file via QFile as below:
    QFile file(m_fileName);

    // and then open it in read only mode.
    if (!file.open(QIODevice::ReadOnly))
    {
        // If there was a problem opening the file, end the import.
        emit message("Couldn't open file");
        emit progress(100);
        emit finished();
        return;
    }

    // With the file open, its contents can be parsed to generate
    // arrays to be registered with iolite.
    while (!file.atEnd())
    {
        // For example, if the data were comma separated, each column
        // for a given row could be read as:
        QStringList lineParts = QString(file.readLine()).split(',');

        // then the individual components as:
        // (note 0-based indexing and conversion to double)
        double column3 = lineParts.at(2).toDouble();

        // and then finally do something with the value, like
        // store it in an array for later.

        // Note! If the file being parsed is not a text-based csv
        // file, there is a very good chance this would crash.
        // Always include error checking to handle things gracefully.
        // We could have for example done something like:
        if (lineParts.count() < 3)
        {
            emit message("Problem parsing file -- not enough columns!");
            emit progress(100);
            emit finished();
            file.close();
            return;
        }
    }

    file.close();

    /* Ultimately, data must end up in armadillo vectors to be
     * registered with iolite. See arma.sourceforge.net for
     * documentation specific to armadillo. The syntax is very
     * similar to matlab. Since we didn't actually read anything
     * above, we'll just make some random data as an example.
     */
    using namespace arma;

    QStringList channelNames = QStringList() << "One" << "Two" << "Three";
    uint numberOfPoints = 100;
    uint currentChannelIndex = 0;
    uint channelCount = channelNames.count();

    // We'll make a common time vector
    vec t = linspace(QDateTime::currentSecsSinceEpoch(), QDateTime::currentSecsSinceEpoch() + 60*60, numberOfPoints);

    // and a random data vector for each of the channel names listed above
    for (auto channelName: channelNames)
    {
        emit message(QString("Working on %1").arg(channelName));
        emit progress(100.0*currentChannelIndex/channelCount);

        vec channel(numberOfPoints, fill::randn);

        emit timeSeriesData("input",        // Type, could be: 'input', 'intermediate', 'output'
                            channelName,    // Channel name
                            QVariantMap(),  // Channel properties
                            channel,        // Vector of data
                            t);             // Vector of time

        currentChannelIndex++;

        // Note that although we're updating the progress and messages,
        // such a trivial example will likely execute too fast
        // to be able to see those messages. To slow things down
        // we can sleep this thread for 2 seconds every loop.
        QThread::sleep(2);
    }

    // Let iolite know we're done...
    emit message("Done");
    emit progress(100);
    emit finished();
}
