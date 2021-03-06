/**
 * @file Universe.h
 * @brief The Universe class.
 * @date January 9, 2012
 * @author William Boyd, MIT, Course 22 (wboyd@mit.edu)
 */

#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#ifdef __cplusplus
#include "Python.h"
#include "constants.h"
#include "LocalCoords.h"
#include "boundary_type.h"
#include <limits>
#include <map>
#include <vector>
#endif


/* Forward declarations to resolve circular dependencies */
class LocalCoords;
class Cell;
class Surface;
class Material;
struct surface_halfspace;


int universe_id();
void reset_universe_id();


/**
 * @enum universeType
 * @brief The type of universe
 */
enum universeType{

  /** A simple non-repeating Universe */
  SIMPLE,

  /** A collection of Universes in a rectangular Lattice */
  LATTICE
};


/**
 * @class Universe Universe.h "src/Universe.h"
 * @brief A Universe represents an unbounded space in the 2D xy-plane.
 * @details A Universe contains cell which are bounded subspaces in the 2D
 *          xy-plane and which together form the Universe. Universes allow
 *          for complex, repeating (i.e. lattices) geometries to be simply
 *          represented with as few data structures as possible.
 */
class Universe {

protected:

  /** A static counter for the number of Universes */
  static int _n;

  /** A monotonically increasing unique ID for each Universe created */
  int _uid;

  /** A user-defined id for each Universe created */
  int _id;

  /** A user-defined name for the Surface */
  char* _name;

  /** The type of Universe (ie, SIMPLE or LATTICE) */
  universeType _type;

  /** A collection of Cell IDs and Cell pointers in this Universe */
  std::map<int, Cell*> _cells;

  /** A boolean representing whether or not this Universe contains a Material
   *  with a non-zero fission cross-section and is fissionable */
  bool _fissionable;

public:

  Universe(const int id=0, const char* name="");
  virtual ~Universe();
  int getUid() const;
  int getId() const;
  char* getName() const;
  universeType getType();
  int getNumCells() const;
  double getMinX();
  double getMaxX();
  double getMinY();
  double getMaxY();
  double getMinZ();
  double getMaxZ();
  boundaryType getMinXBoundaryType();
  boundaryType getMaxXBoundaryType();
  boundaryType getMinYBoundaryType();
  boundaryType getMaxYBoundaryType();
  boundaryType getMinZBoundaryType();
  boundaryType getMaxZBoundaryType();

  Cell* getCell(int cell_id);
  std::map<int, Cell*> getCells() const;
  std::map<int, Cell*> getAllCells();
  std::map<int, Material*> getAllMaterials();
  std::map<int, Universe*> getAllUniverses();
  bool isFissionable();

  void setName(const char* name);
  void setType(universeType type);
  void addCell(Cell* cell);
  void removeCell(Cell* cell);

  Cell* findCell(LocalCoords* coords);
  void setFissionability(bool fissionable);
  void subdivideCells();
  void buildNeighbors();

  std::string toString();
  void printString();

  Universe* clone();
};


/**
 * @class Lattice Universe.h "src/Universe.h"
 * @brief Represents a repeating 2D Lattice of Universes.
 */
class Lattice: public Universe {

private:

  /** The number of Lattice cells along the x-axis */
  int _num_x;

  /** The number of Lattice cells along the y-axis */
  int _num_y;

  /** The width of each Lattice cell (cm) along the x-axis */
  double _width_x;

  /** The width of each Lattice cell (cm) along the y-axis */
  double _width_y;

  /** The coordinates of the offset for the Universe */
  Point _offset;

  /** A container of Universes ? */
  std::vector< std::vector< std::pair<int, Universe*> > > _universes;

public:

  Lattice(const int id=0, const char* name="");
  virtual ~Lattice();

  void setOffset(double x, double y);
  Point* getOffset();
  int getNumX() const;
  int getNumY() const;
  double getWidthX() const;
  double getWidthY() const;
  double getMinX();
  double getMaxX();
  double getMinY();
  double getMaxY();
  double getMinZ();
  double getMaxZ();

  Universe* getUniverse(int lat_x, int lat_y) const;
  std::vector< std::vector< std::pair<int, Universe*> > > getUniverses() const;
  std::map<int, Universe*> getUniqueUniverses();
  std::map<int, Cell*> getAllCells();
  std::map<int, Universe*> getAllUniverses();

  void setNumX(int num_x);
  void setNumY(int num_y);
  void setWidth(double width_x, double width_y);
  void setUniverses(int num_x, int num_y, Universe** universes);
  void buildNeighbors();

  bool withinBounds(Point* point);
  Cell* findCell(LocalCoords* coords);
  double minSurfaceDist(Point* point, double angle);

  int getLatX(Point* point);
  int getLatY(Point* point);

  int getLatticeCell(Point* point);
  int getLatticeSurface(int cell, Point* point);

  std::string toString();
  void printString();
};



/**
 * @brief A helper struct for the Universe::findCell() method.
 * @details This is used to insert a Universe's Cells to the back of a vector
 *          of neighbor Cells in Universe::findCell() routine. This works in
 *          symbiosis with the pair_second method template defined below.
 */
template<typename tPair>
struct second_t {
  typename tPair::second_type operator()(const tPair& p) const {
    return p.second;
  }
};


/**
 * @brief A helper routine for the Universe::findCell() method.
 * @details This is used to insert a Universe's Cells to the back of a vector
 *          of neighbor Cells in Universe::findCell() routine. This works in
 *          symbiosis with the second_t struct template defined above.
 * @param map a std::map iterator
 * @return the second element in the iterator (e.g., map value)
 */
template<typename tMap> 
second_t<typename tMap::value_type> pair_second(const tMap& map) {
  return second_t<typename tMap::value_type>();
}


#endif /* UNIVERSE_H_ */

