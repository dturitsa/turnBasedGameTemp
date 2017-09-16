# Team Zephyr
# Okeanos
## Table of Contents

## Code Styling / Git Managing
### Styilng
K&R Styling
```
    while (x == y) {
        something();
        somethingelse();
    }
```
### Branches
Any new features follow: `feature/{feature name}`  
Bug fixes follow: `hotfix/{issue number}`  
Any ideas that you can think of to implement quick and try follow: `idea/{name}`  
Releases will be tagged: `alpha`, `beta`
### Releases
Any release will be done by the Team Lead and Git Master with the group in a meeting. We all have to conviene to push a release to have verbal confirmation.
Releases follow [branches](#branches).
### Issues
Any issues create an issue and assign it to Spencer. I will then reasses and reassign to a new member to which we can tackle the issue there.
### Pushing/Pulling/Branching
There should always be a new branch when working on anything, and that branch should be [specific work](#branches). **ALWAYS** pull before pushing so as there are no conflicts (or we can at least try to minimize them).
Make a new issue in redmine labelled 'Request: Merge {branch name} into {branch name}' and allow either Git Master or Team Lead to merge.
### Tracking
Milestone, issues, bugs, etc are all tracked through [Redmine](http://btechgmaes.bcit.ca/redmine). Please use this for anything related, label appropriately and give as much detail as you can. Also include how bugs happened, and in what cases so that we can try and fix ASAP with as little downtime as possible.