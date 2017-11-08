# TODO(stek): Use run-clang-tidy

tidyall() {
  for f in *.cpp; do
    clang-tidy "$f" \
      -fix  '-checks=*,-llvm-header-guard' -header-filter '^(includes|test-includes)' -- \
      -std=c++14 -I includes -I test-includes -DNO_DATABASE
  done
}
