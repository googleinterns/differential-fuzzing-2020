Directory contains testing files. Those contain:

    Normal cases:
- anchors.yaml: contains use of aliases with anchors
- anchorscomplex.yaml: more complex cases with multi-level anchors
- global-tag.yaml: test the use of a global tag
- mapping.yaml: test use of maps
- tags.yaml: test the use of tags
- array.yaml: test use of array
- json.yaml: test use of json file (Sequence with map too)
- numbers.yaml: Sequence of numbers (output similar to array.yaml)
- strings.yaml: Sequence with differently formated strings
- yaml-version.yaml: Sequence and map with yaml version
- multimap.yaml: Example for multiple maps
- multisequence.yaml: Example for multiple sequences inside a sequence

    Error cases:
- anchors2.yaml: anchors with an alias without an ancho
- mapseparatevalues.yaml -> 4ABK
- lineasmap.yaml
- errormapping.yaml

- "yaml-cpp needs to parse full doc" error

enddoc.yaml ->3HFZ (end doc error)

enddocandmore.yaml 

tagsrootobject.yaml  -> 35KP (solved)

-

IMPORTANT TEMPORARY ERROR INFO:


specificnonspecifictag.yaml (tag error)

explicitblockmapandemptycase.yaml (? tag error)

-anchor as key

anchorwithcoloninname.yaml  -> 2SXE (weird anchor)

explicitemptykey.yaml (weird tag+weird anchor)

complexsequence.yaml (weird anchor)

anchorsandtokens.yaml (weird anchor)

twoends.yaml (end weird)
-

- (empty key)

mapnoend.yaml (yaml-cpp refuse) (empty key)

mapwithempty.yaml (yaml-cpp refuse) (empty key)
-

unicodeanchor.yaml (libyaml error)



return at 6jwb