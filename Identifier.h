//
// Created by lavorgia on 9/3/16.
//

#ifndef SCHISM_IDENTIFIER_H
#define SCHISM_IDENTIFIER_H

#include <string>
#include <Wretched/System/IDManager.h>

namespace Wretched {

  struct Identifier {
    Identifier() : uid(uids.assign()){
    }
    ~Identifier() {
      uids.remove((int)uid);
    }
    IDManager<long long> uids;
    std::string name;
    const long long uid;
  };
}


#endif //SCHISM_IDENTIFIER_H
