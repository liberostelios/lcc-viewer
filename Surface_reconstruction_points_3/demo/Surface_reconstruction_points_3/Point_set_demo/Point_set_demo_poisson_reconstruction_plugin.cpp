#include "config.h"
#include "Point_set_scene_item.h"
#include "Polyhedron_demo_plugin_helper.h"
#include "Polyhedron_demo_plugin_interface.h"
#include "Scene_polyhedron_item.h"

#include <QObject>
#include <QAction>
#include <QMainWindow>
#include <QApplication>
#include <QtPlugin>
#include <QInputDialog>

#include "ui_Point_set_demo_poisson_reconstruction_plugin.h"

// Poisson reconstruction method:
// Reconstructs a surface mesh from a point set and returns it as a polyhedron.
Polyhedron* poisson_reconstruct(const Point_set& points,
                                FT sm_angle, // Min triangle angle (degrees). 20=fast, 30 guaranties convergence.
                                FT sm_radius, // Max triangle radius w.r.t. point set radius. 0.1 is fine.
                                FT sm_distance); // Approximation error w.r.t. p.s.r.. For Poisson: 0.015=fast, 0.003=smooth.

class Point_set_demo_poisson_reconstruction_plugin :
  public QObject,
  protected Polyhedron_demo_plugin_helper
{
  Q_OBJECT
  Q_INTERFACES(Polyhedron_demo_plugin_interface);

public:
  void init(QMainWindow* mainWindow, Scene_interface* scene_interface) {
    this->scene = scene_interface;
    this->mw = mainWindow;
    actionPoissonReconstruction = this->getActionFromMainWindow(mw, "actionPoissonReconstruction");
    if(actionPoissonReconstruction) {
      connect(actionPoissonReconstruction, SIGNAL(triggered()),
              this, SLOT(reconstruct()));
    }
  }

  QList<QAction*> actions() const {
    return QList<QAction*>() << actionPoissonReconstruction;
  }

public slots:
  void reconstruct();

private:
  QAction* actionPoissonReconstruction;

}; // end class Point_set_demo_poisson_reconstruction_plugin


class Point_set_demo_poisson_reconstruction_plugin_dialog : public QDialog, private Ui::PoissonDialog
{
  Q_OBJECT
  public:
    Point_set_demo_poisson_reconstruction_plugin_dialog(QWidget *parent = 0)
    {
      setupUi(this);
    }

    double triangleAngle() const { return m_inputAngle->value(); }
    double triangleRadius() const { return m_inputRadius->value(); }
    double triangleError() const { return m_inputDistance->value(); }
};

void Point_set_demo_poisson_reconstruction_plugin::reconstruct()
{
  const Scene_interface::Item_id index = scene->mainSelectionIndex();

  Point_set_scene_item* point_set_item =
    qobject_cast<Point_set_scene_item*>(scene->item(index));

  if(point_set_item)
  {
    // Gets point set
    Point_set* points = point_set_item->point_set();
    if(!points) return;

    // Gets options
    Point_set_demo_poisson_reconstruction_plugin_dialog dialog;
    if(!dialog.exec())
      return;
    const double sm_angle     = dialog.triangleAngle();
    const double sm_radius    = dialog.triangleRadius();
    const double sm_distance  = dialog.triangleError();

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Reconstruct point set as a polyhedron
    Polyhedron* pRemesh = poisson_reconstruct(*points, sm_angle, sm_radius, sm_distance);
    if(pRemesh)
    {
      // Add polyhedron to scene
      Scene_polyhedron_item* new_item = new Scene_polyhedron_item(pRemesh);
      new_item->setName(tr("%1 Poisson (%2 %3 %4)")
                         .arg(point_set_item->name())
                         .arg(sm_angle)
                         .arg(sm_radius)
                         .arg(sm_distance));
      new_item->setColor(Qt::magenta);
      scene->addItem(new_item);

      // Hide point set
      point_set_item->setVisible(false);
      scene->itemChanged(index);
    }

    QApplication::restoreOverrideCursor();
  }
}

Q_EXPORT_PLUGIN2(Point_set_demo_poisson_reconstruction_plugin, Point_set_demo_poisson_reconstruction_plugin);

#include "Point_set_demo_poisson_reconstruction_plugin.moc"