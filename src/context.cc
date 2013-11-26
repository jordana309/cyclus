
#include "context.h"

#include "error.h"
#include "timer.h"

namespace cyclus {

Context::Context(Timer* ti, EventManager* em)
    : ti_(ti), em_(em), trans_id_(0) { };

boost::uuids::uuid Context::sim_id() {
  return em_->sim_id();
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Model* Context::GetModelByName(std::string name) {
  Model* found_model = NULL;

  const std::vector<Model*>& models = GetModels();
  
  for (int i = 0; i < models.size(); i++) {
    if (name == models.at(i)->name()) {
      found_model = models.at(i);
      break;
    }
  }

  if (found_model == NULL) {
    std::string err_msg = "Model '" + name + "' doesn't exist.";
    throw KeyError(err_msg);
  }
  return found_model;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Context::PrintModelList() {
  const std::vector<Model*>& models = GetModels();

  CLOG(LEV_INFO1) << "There are " << models.size() << " models.";
  CLOG(LEV_INFO3) << "Model list {";
  for (int i = 0; i < models.size(); i++) {
    CLOG(LEV_INFO3) << models.at(i)->str();
  }
  CLOG(LEV_INFO3) << "}";
}

void Context::AddPrototype(std::string name, Model* p) {
  protos_[name] = p;
}

void Context::AddRecipe(std::string name, Composition::Ptr c) {
  recipes_[name] = c;
};

Composition::Ptr Context::GetRecipe(std::string name) {
  if (recipes_.count(name) == 0) {
    throw KeyError("Invalid recipe name " + name);
  }
  return recipes_[name];
};

void Context::InitTime(int start, int duration, int decay, int m0, int y0,
                       std::string handle) {
  ti_->Initialize(this, duration, m0, y0, start, decay, handle);
};

int Context::time() {
  return ti_->time();
};

int Context::start_time() {
  return ti_->start_time();
};

int Context::sim_dur() {
  return ti_->dur();
};

void Context::RegisterTicker(TimeAgent* ta) {
  ti_->RegisterTickListener(ta);
};

void Context::RegisterResolver(MarketModel* mkt) {
  ti_->RegisterResolveListener(mkt);
};

Event* Context::NewEvent(std::string title) {
  return em_->NewEvent(title);
};

} // namespace cyclus
