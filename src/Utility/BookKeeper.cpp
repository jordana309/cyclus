// BookKeeper.cpp
// Implements the BookKeeper class
#include "BookKeeper.h"
#include "Timer.h"

#include "hdf5.h"
#include "H5Cpp.h"
#include "H5Exception.h"

BookKeeper* BookKeeper::_instance = 0;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
BookKeeper* BookKeeper::Instance()
{
	// If we haven't created a BookKeeper yet, create and return it.
	if (0 == _instance)
		_instance = new BookKeeper();
	
	return _instance;
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
BookKeeper::BookKeeper() 
{
  // we'd like to call the output database cyclus.h5
  dbName = "cyclus.h5";
  const int ncols = 2; 
  const int nrows = LI->getNumFacilities() ;

  string data[nrows][ncols];
  Model* fac;
  for (int i=0; i<nrows; i++){
    fac = LI->getFacilityByID(i);
    data[i][0] = fac->getModelImpl();
    data[i][1] = fac->getName();
  }

  hsize_t dims[2];
  dims[0]= nrows;
  dims[1]= ncols;
  hsize_t rank = 2;

  try{
    // create database. If it already exits, H5F_ACC_TRUNC erases all 
    // data previously stored in the file.
    myDB = new H5File( dbName , H5F_ACC_TRUNC );

    // create groups for the input and output data, respectively
    Group* outGroup = new Group( myDB->createGroup("/output"));
    Group* inGroup = new Group( myDB->createGroup("/input"));

    // create a basic dataset to hold model information
    dbIsOpen = false;

    DataSpace dataspace = DataSpace(rank , dims );

    // create a variable length string types
    StrType vls_type(0, H5T_VARIABLE); 
    DataType datatype = DataType(vls_type);

    DataSet dataset = myDB->createDataSet("/output/test", datatype, dataspace) ; 

    dataset.write(data, datatype);

    delete outGroup;
    delete inGroup;
  }
  catch( FileIException error )
  {
    error.printError();
  }
  catch( GroupIException error )
  {
    error.printError();
  }
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Group* BookKeeper::newGroup(string title){
  // nothing doing...  yet
};


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
DataSpace* newGroup(string title){
  // nothing doing...  yet
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
DataSpace* BookKeeper::homoDataSpace(Group* grp, string name, map< int, pair< string, PredType > > typemap){

  // each dimension needs to use it's knowledge of its predtype 
  // to define the dataspace type
  //
  // this is not the same as with compound types. 
  //
  //
  // the dimensions are the first entries of the map
  vector<hsize_t> dims;

  map<int,pair<string, PredType> >::const_iterator iter=typemap.begin();

  int i = 0;
 
  while (iter != typemap.end()){
    dims[i]=iter->first;
    i++;
    iter++;
  }
  // the length of this map is the rank
  int rank=i;

  // I'm hoping predtypes are already made explicit in the H5CPP api
  DataSpace* toRet = new DataSpace(( rank, dims[1],dims[2]));

  return toRet;
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
DataSet* BookKeeper::fillDataSet(DataSpace* ds, vector<int> data)
{
 // DataSet* toRet;
 // toRet = new DataSet(DataSet(PredType::NATIVE_INT));
 // toRet->write(data);
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::createDB(string name)
{
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
H5File* BookKeeper::getDB()
{
	return myDB;
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::openDB()
{
	// If the database is already open, throw an exception; the caller probably 
	// doesn't realize this.
  try{
    H5File* memDB = new H5File( dbName , H5F_ACC_RDWR );  
  }
  catch( FileIException error )
  {
    error.printError();
  }
	
	// Create datasets to store the information we want? 
	
	// Store the handle to it.
	dbIsOpen = true;
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::closeDB()
{
	// Try to close it.
  try{
    myDB->close();
    dbIsOpen = false;
  }
  // catch failure caused by the H5File operations
  catch( FileIException error )
  {
     error.printError();
  }
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool BookKeeper::isGroup(string grp)
{
  // nothing doing, just yet.
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::writeData(intData1d data, string dsname){ 
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::writeData(intData2d data, string dsname){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::writeData(intData3d data, string dsname){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::writeData(dblData1d data, string dsname){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::writeData(dblData2d data, string dsname){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::writeData(dblData3d data, string dsname){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::writeData(strData1d data, string dsname){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::writeData(strData2d data, string dsname){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::writeData(strData3d data, string dsname){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::readData(DataSpace hs, intData1d& out_data){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::readData(DataSpace hs, intData2d& out_data){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::readData(DataSpace hs, intData3d& out_data){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::readData(DataSpace hs, dblData1d& out_data){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::readData(DataSpace hs, dblData2d& out_data){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::readData(DataSpace hs, dblData3d& out_data){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::readData(DataSpace hs, strData1d& out_data){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::readData(DataSpace hs, strData2d& out_data){
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BookKeeper::readData(DataSpace hs, strData3d& out_data){
};