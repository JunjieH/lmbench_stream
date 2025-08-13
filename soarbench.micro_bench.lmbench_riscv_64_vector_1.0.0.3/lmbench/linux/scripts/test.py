import os
path = "../bin"


data_size = ["8192","32768","131072","262144","1048576","4194304","8388608","33554432","268435456"]

int_pattern = ["rd","cross_cacheline_rd","non_stride0_rd","non_stride1_rd","non_stride2_rd","unfixed_stride_rd","parallel_cross_cacheline_rd","parallel_non_stride0_rd","parallel_non_stride1_rd","parallel_non_stride2_rd","parallel_unfixed_stride_rd"]
float_pattern = ["cross_cacheline_rd","non_stride0_rd","non_stride1_rd","non_stride2_rd","unfixed_stride_rd","parallel_cross_cacheline_rd","parallel_non_stride0_rd","parallel_non_stride1_rd","parallel_non_stride2_rd","parallel_unfixed_stride_rd"]
double_pattern = ["cross_cacheline_rd","non_stride0_rd","non_stride1_rd","non_stride2_rd","unfixed_stride_rd","parallel_cross_cacheline_rd","parallel_non_stride0_rd","parallel_non_stride1_rd","parallel_non_stride2_rd","parallel_unfixed_stride_rd"]
long_pattern = ["cross_cacheline_rd","non_stride0_rd","non_stride1_rd","non_stride2_rd","unfixed_stride_rd","parallel_cross_cacheline_rd","parallel_non_stride0_rd","parallel_non_stride1_rd","parallel_non_stride2_rd","parallel_unfixed_stride_rd"]

os.chdir(path)

print("Test the int patterns:\n")
for j in int_pattern:
  print("Test the %s pattern, results are as follows: " % j)
  for i in data_size:
    os.system("./bw_mem -W 4 -N 10 " + i + " " + j)

print("Test the float patterns:\n")
for j in float_pattern:
  print("Test the %s pattern, results are as follows: " % j)
  for i in data_size:
    os.system("./bw_mem_float -W 4 -N 10 " + i + " " + j)

print("Test the double patterns:\n")
for j in double_pattern:
  print("Test the %s pattern, results are as follows: " % j)
  for i in data_size:
    os.system("./bw_mem_double -W 4 -N 10 " + i + " " + j)

print("Test the long patterns:\n")
for j in long_pattern:
  print("Test the %s pattern, results are as follows: " % j)
  for i in data_size:
    os.system("./bw_mem_long -W 4 -N 10 " + i + " " + j)


