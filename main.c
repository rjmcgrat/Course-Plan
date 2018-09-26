// Rodney McGrath
// 55352053
// Lab 1

//constants
#define MAX_NUM_COURSES_GRADUATE 100
#define MAX_COURSE_NAME_LEN 20
#define MAX_CHAR_READ_PER_LINE 512
#define MAX_QUARTERS_PER_YEAR 3
#define MAX_YEARS_GRADUATE 10
#define MAX_PREREQS_PER_COURSE 5
#define MAX_COURSES_OFFERED 20


//libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//helper functions
int countWords(char *s){
  int count = 0;
  int i;
  for (i = 0; s[i] != '\0'; i++){
    if (s[i] == ' ' ||
      s[i+1] == '\0')
        count++;
  }
  return count;
}
int courseExistsInArray(char array[MAX_COURSES_OFFERED][MAX_COURSE_NAME_LEN],
  int sizeArray, char *course){
    int i;
    for (i = 0; i < sizeArray; i++){
      if(strcmp(array[i], course) == 0){
        return 1;
      }
  }
  return 0;
}

int noZerosPrq(int array[MAX_COURSES_OFFERED], int sizeArray){
  int i;
  for (i = 0; i < sizeArray; i++) {
      if (array[i] == 0)
          return 0;
  }
  return 1;
}

//main
int main(int argc, char **argv){
  //Read files
  FILE  *f1;
  f1 = fopen(argv[1], "r");
  FILE *f2;
  f2 = fopen(argv[2], "r");
  FILE *f3;
  f3 = fopen(argv[3], "r");

  //Offering Information
  int i,i2;
  typedef struct offeredCourses
  {
    char quarter;
    char courseNames[MAX_COURSES_OFFERED][MAX_COURSE_NAME_LEN];
    int sizeCourseNames;
  } oc;
  oc oc1, oc2, oc3;
  oc1.quarter = 1;
  oc2.quarter = 2;
  oc3.quarter = 3;
  oc1.sizeCourseNames = oc2.sizeCourseNames = oc3.sizeCourseNames = 0;

  char offeringLine[MAX_CHAR_READ_PER_LINE];
  if (f1 == NULL){
    perror("Error opening file.\n");
    exit(0);
  }
  while(fgets(offeringLine, MAX_CHAR_READ_PER_LINE , f1) != NULL){


    char *courseToken = strtok(offeringLine, " \t\n");
    char quarterToken = (strtok(NULL, " \t\n")[0]) - '\0';


    if(quarterToken == '1'){
      strcpy(oc1.courseNames[oc1.sizeCourseNames], courseToken);
      oc1.sizeCourseNames++;
    }
    else if(quarterToken == '2'){
      strcpy(oc2.courseNames[oc2.sizeCourseNames], courseToken);
      oc2.sizeCourseNames++;
    }
    else if(quarterToken == '3'){
      strcpy(oc3.courseNames[oc3.sizeCourseNames], courseToken);
      oc3.sizeCourseNames++;
    }
  }
  fclose(f1);

  //Prerequisites Information
  struct prereqs{
    char courseNames[MAX_PREREQS_PER_COURSE][MAX_COURSE_NAME_LEN];
    int sizeCourseNames;
    int pqCourseNames[MAX_PREREQS_PER_COURSE];
  };

  struct prerequisitesInfo
  {
    char courseName[MAX_COURSE_NAME_LEN];
    struct prereqs prq;
    int courseTaken;
    int metAllPrequisites;
  }pi;

  char prereqLine[MAX_CHAR_READ_PER_LINE];
  if (f2 == NULL){
    perror("Error opening file.\n");
    exit(0);
  }
  struct prerequisitesInfo arrayPI[MAX_COURSES_OFFERED];
  int sizeArrayPI;
  sizeArrayPI = 0;
  while(fgets(prereqLine, MAX_CHAR_READ_PER_LINE, f2) != NULL){
      struct prereqs pqForInfo;
      pqForInfo.sizeCourseNames = 0;
      struct prerequisitesInfo structName;
      structName.metAllPrequisites = structName.courseTaken = 0;
      char *token = strtok(prereqLine, " \t\n\r");
      strcpy(structName.courseName, token);
      token = strtok(NULL, " \t\n\r");
      if(token == NULL){
        structName.metAllPrequisites = 1;
      }
      while(token != NULL){
        strcpy(pqForInfo.courseNames[pqForInfo.sizeCourseNames], token);
        pqForInfo.pqCourseNames[pqForInfo.sizeCourseNames] = 0;
        token = strtok(NULL, " \t\n\r");
        pqForInfo.sizeCourseNames++;
      }
      structName.prq = pqForInfo;
      arrayPI[sizeArrayPI] = structName;
      sizeArrayPI++;
    }
    // for(i = 0; i < sizeArrayPI; i++){
    //   printf("MasterCourse: %s\tPreqs:", arrayPI[i].courseName);
    //   for (i2 = 0; i2 < arrayPI[i].prq.sizeCourseNames; i2++){
    //     printf(" %s", arrayPI[i].prq.courseNames[i2]);
    //   }
    //   printf("\n");
    // }
  fclose(f2);

  //Requirement Information
  int numberCourses;
  numberCourses = 0;
  char requiredCoursesLine[MAX_CHAR_READ_PER_LINE];
  char requiredCourses[MAX_NUM_COURSES_GRADUATE][MAX_COURSE_NAME_LEN];
  if (f3 == NULL){
    perror("Error opening file");
    exit(0);
  }
  i = 0;
  while(fgets(requiredCoursesLine, MAX_CHAR_READ_PER_LINE, f3) != NULL){
    numberCourses += countWords(requiredCoursesLine);
    char *requiredCoursesToken = strtok(requiredCoursesLine, " \t\n");
    strcpy(requiredCourses[i], requiredCoursesToken);
    requiredCoursesToken = strtok(NULL, " \t\n");
    while(requiredCoursesToken != NULL){
      ++i;
      strcpy(requiredCourses[i], requiredCoursesToken);
      requiredCoursesToken = strtok(NULL, " \t\n");
    }
  }
  fclose(f3);

  //Courses to Take
  int z,zz;
  char courseBank[MAX_COURSES_OFFERED][MAX_COURSE_NAME_LEN];
  int numBank = 0;
  for(i = 0; i < numberCourses; i++){
    if(courseExistsInArray(courseBank, numBank, requiredCourses[i]) == 0){
      strcpy(courseBank[numBank], requiredCourses[i]);
      numBank++;
    }
    for(z = 0; z < sizeArrayPI; z++){
      for(zz = 0; zz < arrayPI[z].prq.sizeCourseNames; zz++){
        if(courseExistsInArray(courseBank, numBank,
          arrayPI[z].prq.courseNames[zz]) == 0){
            strcpy(courseBank[numBank], arrayPI[z].prq.courseNames[zz]);
            numBank++;
        }
      }
    }
  }





  int ccb = 0;
  for (i2 = 0; i2 < numBank; i2++){
    for(i = 0; i < sizeArrayPI; i++){
      if (strcmp(courseBank[i2], arrayPI[i].courseName) == 0){
        ccb = 1;
        break;
      }
    }
    if (ccb == 0){
      struct prerequisitesInfo structName;
      struct prereqs pqForInfo;
      strcpy(structName.courseName, courseBank[i2]);
      structName.metAllPrequisites = 1;
      structName.courseTaken = 0;
      structName.prq = pqForInfo;
      arrayPI[sizeArrayPI] = structName;
      sizeArrayPI++;
    }
    ccb = 0;
  }






  //Course Plan
  int y, q, j, k;
  y = 1;
  char coursePlan[MAX_COURSES_OFFERED][MAX_COURSE_NAME_LEN];
  char coursePlanned[MAX_COURSE_NAME_LEN];
  int numPlan;
  numPlan = 0;
  for (q = 1; q <= MAX_QUARTERS_PER_YEAR; q++){
    if(numPlan == numBank || y>10){
      break;
    }
    if(numPlan <= numBank){
      printf("%d\t%d", y, q);
      if (q == 1){
        for (j = 0; j < oc1.sizeCourseNames; j++){
          // printf("CourseNameOffered: %s ->", oc1.courseNames[j]);
          for(zz = 0; zz < numBank; zz++){
            // printf(" courseBank:%s", courseBank[zz]);
            if (strcmp(courseBank[zz], oc1.courseNames[j]) == 0) {
                for(k = 0; k < sizeArrayPI; k++){
                  if(strcmp(courseBank[zz], arrayPI[k].courseName)
                    == 0 && arrayPI[k].courseTaken == 0
                      && arrayPI[k].metAllPrequisites == 1){
                        arrayPI[k].courseTaken = 1;
                        strcpy(coursePlan[numPlan], arrayPI[k].courseName);
                        strcpy(coursePlanned, arrayPI[k].courseName);
                        printf("\t%s", coursePlan[numPlan]);
                        numPlan++;
                        for (k = 0; k < sizeArrayPI; k++){
                          for (zz = 0; zz < arrayPI[k].prq.sizeCourseNames; zz++){
                            if(strcmp(coursePlanned, arrayPI[k].prq.courseNames[zz]) == 0){
                              // printf("Course Planned: %s\t", coursePlanned);
                              // printf("prereq of: %s\n", arrayPI[k].courseName);
                              arrayPI[k].prq.pqCourseNames[zz] = 1;
                            }
                          }

                        }
                  }
                }
              }
          }
          // printf("\n");

          }
          for (k = 0; k < sizeArrayPI; k++){
            for (zz = 0; zz < arrayPI[k].prq.sizeCourseNames; zz++){
              if(noZerosPrq(arrayPI[k].prq.pqCourseNames,
                arrayPI[k].prq.sizeCourseNames) == 1){
                  arrayPI[k].metAllPrequisites = 1;
              }
            }
        }
          printf("\n");
        }
      else if (q == 2){
        for (j = 0; j < oc2.sizeCourseNames; j++){
          // printf("CourseNameOffered: %s ->", oc2.courseNames[j]);
          for(zz = 0; zz < numBank; zz++){
            if (strcmp(courseBank[zz], oc2.courseNames[j]) == 0) {
                for(k = 0; k < sizeArrayPI; k++){
                  if(strcmp(courseBank[zz], arrayPI[k].courseName)
                    == 0 && arrayPI[k].courseTaken == 0
                      && arrayPI[k].metAllPrequisites == 1){
                        arrayPI[k].courseTaken = 1;
                        strcpy(coursePlan[numPlan], arrayPI[k].courseName);
                        strcpy(coursePlanned, arrayPI[k].courseName);
                        printf("\t%s", coursePlan[numPlan]);
                        numPlan++;
                        for (k = 0; k < sizeArrayPI; k++){
                          for (zz = 0; zz < arrayPI[k].prq.sizeCourseNames; zz++){
                            if(strcmp(coursePlanned, arrayPI[k].prq.courseNames[zz]) == 0){
                              // printf("Course Planned: %s\t", coursePlanned);
                              // printf("prereq of: %s\n", arrayPI[k].courseName);
                              arrayPI[k].prq.pqCourseNames[zz] = 1;
                            }
                          }
                        }
                  }
                }
              }
          }
          // printf("\n");

          }
          for (k = 0; k < sizeArrayPI; k++){
            for (zz = 0; zz < arrayPI[k].prq.sizeCourseNames; zz++){
              if(noZerosPrq(arrayPI[k].prq.pqCourseNames,
                arrayPI[k].prq.sizeCourseNames) == 1){
                  arrayPI[k].metAllPrequisites = 1;
              }
            }
        }
          printf("\n");
          }
        else if (q == 3){
          for (j = 0; j < oc3.sizeCourseNames; j++){
            // printf("CourseNameOffered: %s ->", oc3.courseNames[j]);
            for(zz = 0; zz < numBank; zz++){
              // printf(" courseBank:%s", courseBank[zz]);
              if (strcmp(courseBank[zz], oc3.courseNames[j]) == 0) {
                  for(k = 0; k < sizeArrayPI; k++){
                    if(strcmp(courseBank[zz], arrayPI[k].courseName)
                      == 0 && arrayPI[k].courseTaken == 0
                        && arrayPI[k].metAllPrequisites == 1){
                          arrayPI[k].courseTaken = 1;
                          strcpy(coursePlan[numPlan], arrayPI[k].courseName);
                          strcpy(coursePlanned, arrayPI[k].courseName);
                          printf("\t%s", coursePlan[numPlan]);
                          numPlan++;
                          for (k = 0; k < sizeArrayPI; k++){
                            for (zz = 0; zz < arrayPI[k].prq.sizeCourseNames; zz++){
                              if(strcmp(coursePlanned, arrayPI[k].prq.courseNames[zz]) == 0){
                                // printf("Course Planned: %s\t", coursePlanned);
                                // printf("prereq of: %s\n", arrayPI[k].courseName);
                                arrayPI[k].prq.pqCourseNames[zz] = 1;
                              }
                            }

                          }
                    }
                  }
                }
            }
            // printf("\n");
            }
            for (k = 0; k < sizeArrayPI; k++){
              for (zz = 0; zz < arrayPI[k].prq.sizeCourseNames; zz++){
                if(noZerosPrq(arrayPI[k].prq.pqCourseNames,
                  arrayPI[k].prq.sizeCourseNames) == 1){
                    arrayPI[k].metAllPrequisites = 1;
                }
              }
          }
            printf("\n");

          }
          if (q == MAX_QUARTERS_PER_YEAR){
            q = 0;
            y++;
          }


       }
  }
  // printf("numBanl: %d\n", numBank);
  // printf("numPlan: %d\n", numPlan);
  // for(k = 0; k < numPlan; k++){
  //   printf("%s\n", coursePlan[k]);
  // }
  return 0;
}
