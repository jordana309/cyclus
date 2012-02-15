// Material.h
#if !defined(_MATERIAL_H)
#define _MATERIAL_H

#include <map>
#include <utility>
#include <math.h>
#include <vector>
#include <string>
#include <libxml/tree.h>

#include "UseMatrixLib.h"
#include "Resource.h"
#include "IsoVector.h"

#define WF_U235 0.007200 // feed, natural uranium 

class Material;
typedef boost::intrusive_ptr<Material> mat_rsrc_ptr;

/*!
   @class Material

   @brief The Cyclus Material class provides the data structure that supports 
   the isotopic composition of materials passed around in a Cyclus simulation 
   and the functions as the interface with which models interact 
   with materials.

   @section intro Introduction

   This class keeps track of the isotopic composition of a material.
   Composition can be queried as either atom-based (moles) or mass-based (kg).
   The material state is book-kept each time a material object is transacted
   between simulation agents.

   @section dataTables Data Tables
   Material data can be added with a Table class. This is currently under 
   development for the MassTable.

   @subsection interface Interface Models in a Cyclus simulation must interact
   with materials in order to perform their fuel cycle tasks. Some models will
   need to create, destroy, query and alter materials and their compositions.
   In order to do this, the Material class, in concert with the IsoVector
   class, provides an interface that supports these actions by any model that
   includes its header file.
   
   @section modifyMaterials Modifying Materials

   Most interaction with material objects will take the form of checking their
   composition properties by querying the material object's stored IsoVector.
   The only methods available to modify the material object are the 'absorb'
   and 'extract' methods.  Changing a material object's composition is not
   possible.  If the composition needs to be changed, a new material object
   will need to be created with the desired composition.  In general, the
   setQuantity method should NOT be used to change the mass of a material
   object. An IsoVector containing the desired composition and mass properties
   should be configured prior to the material object creation

*/
class Material : public Resource {

public:
  Material();

  /*!
   a constructor for making a material object from a known recipe and size.
   
   @param comp isotopic makeup of this material object
   */
  Material(IsoVector comp);

  Material(const Material& other);
  
  ~Material() {};

  
  /**
   * standard verbose printer includes both an atom and mass composition output
   */
  void print(); 
    
  /**
   * Change/set the mass of the resource object. Note that this does make
   * matter (dis)appear and should only be used on objects that are not part of
   * any actual tracked inventory.
   */
  void setQuantity(double quantity);

  /// Resource class method
  double quantity() {return iso_vector_.mass();};

  /// Resource class method
  std::string units() {return "kg";};

  /// Resource class method
  bool checkQuality(rsrc_ptr other);

  /// Resource class method
  bool checkQuantityEqual(rsrc_ptr other);

  /// Resource class method
  bool checkQuantityGT(rsrc_ptr other);

  /// Resource class method
  ResourceType type() {return MATERIAL_RES;};

  /// Resource class method
  rsrc_ptr clone();

  /*!
   Absorbs the contents of the given Material into this Material and deletes 
   the given Material. 
   
   @param matToAdd the Material to be absorbed (and deleted)
   */
  virtual void absorb(mat_rsrc_ptr matToAdd);

  /*!
   Extracts from this material a composition specified by the given IsoVector
   
   @param rem_comp the composition/amount of material that will be removed
   against this Material
   
   @return the extracted material as a newly allocated material object
   */
  virtual mat_rsrc_ptr extract(IsoVector rem_comp);

  /*!
   Extracts a specified mass from this material creating a new material
   object with the same isotopic ratios.
   
   @param the amount (mass) of material that will be removed
   
   @return the extracted material as a newly allocated material object
   */
  virtual mat_rsrc_ptr extract(double mass);

  /*!
   Decays this Material object for the given number of months and updates
   its composition map with the new number densities.
   
   @param months the number of months to decay
   */
  void decay(double months);
  
  /*!
   Decays this Material object for however many months have passed since the 
   last entry in the material history.
   
   */
  void decay();

  /*!
  Returns a copy of this material's isotopic composition
  */
  IsoVector isoVector() {return iso_vector_;}

  /*!
   Decays all of the materials if decay is on
   
   @todo should be private (khuff/rcarlsen)
   
   @param time is the simulation time of the tick
   */
  static void decayMaterials(int time);
  
  /// sets the decay boolean and the interval
  static void setDecay(int dec);

private:

  /// last time this material object's state was accurate (e.g. time of last
  /// decay, etc.)
  int last_update_time_;

  /// all isotopic details of this material object
  IsoVector iso_vector_;

  /// list of materials
  static std::vector<mat_rsrc_ptr> materials_;

  /// true if decay should occur, false if not.
  static bool decay_wanted_;

  /// how many months between decay calculations
  static int decay_interval_;

};

#endif
