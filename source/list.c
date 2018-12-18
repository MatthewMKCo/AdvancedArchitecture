#include "run.h"
#include <string.h>

void replacedata(node *node, int registerNumber);

//creates a new ring
ring *createring(char* name){
  ring *r = malloc(sizeof(ring));
  r -> name = name;
  node *sentinelnode = malloc(sizeof(node));
  sentinelnode -> data.tagNumber = -1;
  sentinelnode -> data.registerNumber = -1;
  sentinelnode -> id = -1;
  sentinelnode -> name = name;
  sentinelnode -> ready = 0;

  r -> sentinel = sentinelnode;
  r -> last = sentinelnode;
  sentinelnode -> forward = sentinelnode;
  sentinelnode -> back = sentinelnode;
  r -> first = sentinelnode -> forward;
  return r;
}

//adds a node after the last node
void addafternode(ring *currentRing, tag data){
  node *newnode = malloc(sizeof(node));
  newnode -> name = currentRing -> name;
  newnode -> data = data;
  currentRing -> last -> forward = newnode;
  currentRing -> sentinel -> back = newnode;
  newnode -> forward = currentRing -> sentinel;
  newnode -> back = currentRing -> last;
  currentRing -> last = newnode;
  if(currentRing -> first -> data.tagNumber == -1){
    currentRing -> first = newnode;
  }
}

//adds a node after the last node
void addafternodeinstruction(ring *currentRing, uint32_t instruction, int id, tag tagData){
  node *newnode = malloc(sizeof(node));

  newnode -> name = currentRing -> name;
  newnode -> data = tagData;
  newnode -> instruction = instruction;
  newnode -> id = id;
  newnode -> ready = 0;
  currentRing -> last -> forward = newnode;
  newnode -> forward = currentRing -> sentinel;
  newnode -> back = currentRing -> last;
  if(currentRing -> first -> data.tagNumber == -1){
    currentRing -> first = newnode;
  }
  currentRing -> sentinel -> forward = currentRing -> first;
  if(currentRing -> last -> data.tagNumber == -1){
    currentRing -> last = newnode;
  }
  currentRing -> sentinel -> back = newnode;

  currentRing -> last = newnode;
}

void movenode2(ring *inuse, node *oldFirst){
  node *before = inuse -> last;

  inuse -> last -> forward = oldFirst;
  inuse -> sentinel -> back = oldFirst;
  inuse -> last = oldFirst;
  oldFirst -> forward = inuse -> sentinel;
  oldFirst -> back = before;
  if(inuse -> first == inuse -> sentinel){
    inuse -> first = oldFirst;
    inuse -> last = oldFirst;
    inuse -> sentinel -> forward = oldFirst;
    inuse -> sentinel -> back = oldFirst;
  }
}

//deletes the first node
int movenode(ring *unused, ring *inuse, int registerNumber){
  node *before = unused -> first -> back;
  node *after = unused -> first -> forward;
  node *oldFirst = unused -> first;
  if(oldFirst -> data.tagNumber == -1){
    // printring(inuseTags);
    // printring(unusedTags);
    // printf("ERROR in movenode for %s\n", unused -> name);
    return oldFirst -> data.tagNumber;
    // exit_early(1);
  }

  unused -> first = oldFirst -> forward;
  unused -> first -> back = before;
  before -> forward = after;
  unused -> last = unused -> sentinel -> back;
  // printf("%d\n", registerNumber);
  // printf("%s\n", unused -> name);
  // int ret = strcmp(unused -> name, "inuseTags")
  // if(oldFirst -> data.tagNumber == 13 ){
  //   exit_early();
  // }

  replacedata(oldFirst, registerNumber);
  oldFirst -> name = inuse -> name;
  movenode2(inuse, oldFirst);

  return oldFirst -> data.tagNumber;
}

//deletes the selected node after finding tag
int moveselectednode(ring *unused, ring *inuse, int registerNumber, int findTagNumber){
  unused -> selected = unused -> first;
  while(1){
    if(unused -> selected == unused -> sentinel){
      printring(inuseTags);
      printring(unusedTags);
      printf("ERROR in moveselectednode for %s\n", unused -> name);
      exit_early(1);
    }
    if(unused -> selected -> data.tagNumber == findTagNumber)break;
    else next(unused);
  }

  node *before = unused -> selected -> back;
  node *after = unused -> selected -> forward;
  node *oldSelected = unused -> selected;
  if(oldSelected -> data.tagNumber == -1){
    printring(inuseTags);
    printring(unusedTags);
    printf("ERROR in movenode for %s\n", unused -> name);
    exit_early(1);
  }

  unused -> selected = oldSelected -> forward;
  unused -> selected -> back = before;
  before -> forward = after;
  unused -> first = unused -> sentinel -> forward;
  unused -> last = unused -> sentinel -> back;
  // printf("%d\n", registerNumber);
  // printf("%s\n", unused -> name);
  // int ret = strcmp(unused -> name, "inuseTags")
  // if(oldFirst -> data.tagNumber == 13 ){
  //   exit_early();
  // }

  replacedata(oldSelected, registerNumber);
  oldSelected -> name = inuse -> name;
  movenode2(inuse, oldSelected);

  return oldSelected -> data.tagNumber;
}




//Get the tag at the first position
tag get(ring *currentRing){
  tag value = currentRing -> first -> data;
  return value;
}

tag getSelected(ring *currentRing){
  tag value = currentRing -> selected -> data;
  return value;
}

int getinstructionid(ring *currentRing){
  int id = currentRing -> first -> id;
  return id;
}

void replacedata(node *node, int registerNumber){
  node -> data.registerNumber = registerNumber;
  return;
}

void next(ring *currentRing){
  currentRing -> selected = currentRing -> selected -> forward;
  return;
}

void back(ring *currentRing){
  currentRing -> selected = currentRing -> selected -> back;
  return;
}

void start(ring *currentRing){
  currentRing -> selected = currentRing -> first;
}

void last(ring *currentRing){
  currentRing -> selected = currentRing -> sentinel -> back;
}

find find_tag(ring *currentRing, int tagNumber){
  currentRing -> selected = currentRing -> first;
  find foundtag;
  while(currentRing -> selected -> data.tagNumber != -1){
    if(currentRing -> selected -> data.tagNumber == tagNumber){
      foundtag.found = 1;
      foundtag.number = tagNumber;
      return foundtag;
    }
    next(currentRing);
  }
  foundtag.found = -1;
  foundtag.number = -1;
  return foundtag;
}

find find_id(ring *currentRing, int id){
  currentRing -> selected = currentRing -> first;
  find foundtag;
  while(currentRing -> selected != currentRing -> sentinel){
    if(currentRing -> selected -> id == id){
      foundtag.found = 1;
      foundtag.number = id;
      return foundtag;
    }
    next(currentRing);
  }
  foundtag.found = -1;
  foundtag.number = -1;
  return foundtag;
}


find find_register(ring *currentRing, int registerNumber){
  currentRing -> selected = currentRing -> first;
  find foundtag;
  while(currentRing -> selected -> data.registerNumber != -1){
    if(currentRing -> selected -> data.registerNumber == registerNumber){
      foundtag.found = 1;
      foundtag.number = currentRing -> selected -> data.tagNumber;
      return foundtag;
    }
    next(currentRing);
  }
  foundtag.found = -1;
  foundtag.number = -1;
  return foundtag;
}

find find_register_last(ring *currentRing, int registerNumber){
  currentRing -> selected = currentRing -> last;
  find foundtag;
  while(currentRing -> selected -> data.registerNumber != -1){
    if(currentRing -> selected -> data.registerNumber == registerNumber){
      foundtag.found = 1;
      foundtag.number = currentRing -> selected -> data.tagNumber;
      return foundtag;
    }
    back(currentRing);
  }
  foundtag.found = -1;
  foundtag.number = -1;
  return foundtag;
}


find check_first(ring *currentRing, ring *secondRing, int tagDestination){
  find foundtag;
  if(currentRing -> first -> data.tagNumber == tagDestination){
    foundtag.found = 1;
    foundtag.number = currentRing -> first -> data.registerNumber;
    return foundtag;
  }
  else{
    foundtag.found = 0;
    foundtag.number = -1;
    return foundtag;
  }
}

find second_last(ring *currentRing, int registerDestination){
  last(currentRing);
  find foundtag;
  int numberofTimesFound = 0;
  while(1){
    if(currentRing -> selected == currentRing -> sentinel){
      foundtag.found = 0;
      foundtag.number = -1;
      return foundtag;
    }
    if(currentRing -> selected -> data.registerNumber == registerDestination){
      numberofTimesFound++;
    }
    if(numberofTimesFound == 2){
      foundtag.found = 1;
      foundtag.number = currentRing -> selected -> data.tagNumber;
      return foundtag;
    }
    back(currentRing);
  }
}

void deletenode(ring *currentRing){
  if(currentRing -> selected -> data.tagNumber == -1){
    printf("Deleting Sentinel, error");
    exit(1);
  }
  node *before = currentRing -> selected -> back;
  node *oldSelected = currentRing -> selected;
  currentRing -> selected = currentRing -> selected -> forward;
  currentRing -> selected -> back = before;
  before -> forward = currentRing -> selected;
  free(oldSelected);
  currentRing -> first = currentRing -> sentinel -> forward;
  currentRing -> last = currentRing -> sentinel -> back;
}

void printring(ring *currentRing){
  printf("Current Ring Name:%s\n", currentRing -> name);
  currentRing -> selected = currentRing -> first;
  while(1){
    if(currentRing -> selected -> data.tagNumber == -1){
      printf("Id:%d\tTag:%d\tRegister:%d\tReady:%d\tName:%s\n", currentRing -> selected -> id, currentRing -> selected -> data.tagNumber, currentRing -> selected -> data.registerNumber, currentRing -> selected -> ready,currentRing -> selected -> name);
      break;
    }
    printf("Id:%d\tTag:%d\tRegister:%d\tReady:%d\tName:%s\n", currentRing -> selected -> id, currentRing -> selected -> data.tagNumber, currentRing -> selected -> data.registerNumber, currentRing -> selected -> ready,currentRing -> selected -> name);
    next(currentRing);
  }
  printf("Id of first:%d\n", currentRing -> first -> id);

  printf("Id of last:%d\n", currentRing -> last -> id);
  printf("\n");
  currentRing -> selected = currentRing -> first;
}

void printringbackwards(ring *currentRing){
  printf("Current Ring Name:%s\n", currentRing -> name);
  currentRing -> selected = currentRing -> last;
  while(1){
    if(currentRing -> selected -> data.registerNumber == -1){
      printf("Register:%d\tName:%s\n", currentRing -> selected -> id, currentRing -> selected -> name);
      break;
    }
    printf("Register:%d\tName:%s\n", currentRing -> selected -> data.registerNumber, currentRing -> selected -> name);
    back(currentRing);
  }
  printf("Id of first:%d\n", currentRing -> first -> id);

  printf("Id of last:%d\n", currentRing -> last -> id);
  printf("\n");
  currentRing -> selected = currentRing -> first;
}

void change_selected_to_first(ring* currentRing){
  currentRing -> selected = currentRing -> first;
}

everything get_everything(ring *currentRing){
  everything value;
  value.id = currentRing -> selected -> id;
  value.tagData = currentRing -> selected -> data;
  value.instruction = currentRing -> selected -> instruction;
  value.ready = currentRing -> selected -> ready;
  return value;
}

void change_to_ready(ring* currentRing){
  start(currentRing);
  while(currentRing -> selected != currentRing -> sentinel){
    currentRing -> selected -> ready = 1;
    next(currentRing);
  }
  return;
}

void deletenodeswithgreaterthanid(int purgeid){
  outOfOrderInstructions -> selected = outOfOrderInstructions -> first;
  while(1){
    if(outOfOrderInstructions -> selected == outOfOrderInstructions -> sentinel){
      break;
    }
    if(outOfOrderInstructions -> selected -> id > purgeid){
      deletenode(outOfOrderInstructions);
    }
    else next(outOfOrderInstructions);
  }

  inOrderInstructions -> selected = inOrderInstructions -> first;
  while(1){
    if(inOrderInstructions -> selected == inOrderInstructions -> sentinel){
      break;
    }
    if(inOrderInstructions -> selected -> id > purgeid){
      if(inOrderInstructions -> selected -> data.tagNumber >= 0){
        moveselectednode(inuseTags, unusedTags, -1, inOrderInstructions -> selected -> data.tagNumber);
      }
      deletenode(inOrderInstructions);
    }
    else next(inOrderInstructions);
  }
}
