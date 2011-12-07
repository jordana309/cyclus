// Resource.h
#if !defined(_RESOURCE_H)
#define _RESOURCE_H

class Resource {
  /**
   * A boolean comparing the quality of the second resource 
   * to the quality of the first 
   *
   * @param first The base resource
   * @param second The resource to evaluate
   *
   * @return True if second is sufficiently equal in quality to 
   * first, False otherwise.
   */
  virtual bool checkQuality(Resource* first, Resource* second)=0;

  /**
   * A boolean comparing the quantity of the second resource is 
   * to the quantity of the first 
   *
   * @param first The base resource
   * @param second The resource to evaluate
   *
   * @return True if second is sufficiently equal in quantity to 
   * first, False otherwise.
   */
  virtual bool checkQuantityEqual(Resource* first, Resource* second)=0;

  /**
   * Returns true if the quantity of the second resource is 
   * greater than the quantity of the first 
   *
   * @param first The base resource
   * @param second The resource to evaluate
   *
   * @return True if second is sufficiently equal in quantity to 
   * first, False otherwise.
   */
  virtual bool checkQuantityGT(Resource* first, Resource* second)=0;

  /**
   * Compares the quantity and quality of the second resource  
   * to the first 
   *
   * @param first The base resource
   * @param second The resource to evaluate
   *
   * @return True if second is sufficiently equal to the first, 
   * False otherwise.
   */
  virtual bool checkEquality(Resource* first, Resource* second){
    bool toRet;
    (checkQuality(first,second) && checkQuantityEqual(first,second)) ? toRet = true : toRet = false;
    return toRet;
  }
};

#endif
