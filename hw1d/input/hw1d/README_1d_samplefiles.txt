Provided are a few sample files.

hw1d_first_sample.txt is the same as from hw1b, with slightly different lighting parameters.
It is not important that you are able to get exactly the same output as my provided
ones, but zoom in on the pictures and see any subtle differences that do or do not
arise betweenst them. The outputs from this are reflection_depth2, reflection_depth3,
and reflection_depth5. 

hw1d_second_sample_lesslighting.txt is similar to the first sample, but now only
has one light. I also changed my program not to use light colours the same way,
so you can see it lit much in the same fashion as the 1c samples were. Depths of
2, 3, and 5 are provided.

hw1d_refraction_sample.txt relies on the rainbow stripes.ppm, so you'll need
to have texture loading working. It also uses triangles, so if your triangles weren't
working, you'll need to replace the triangles with spheres to see what happens
with the refraction. If your triangles merely weren't textured, it should still work,
just with a different generated image.

You can either add a flag to your program to take in a depth
either via commandline args, input file declaration, or just recompile every time
for a new depth. Document what you do.