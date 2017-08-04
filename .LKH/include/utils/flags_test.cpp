#include "flags.cpp"

void test_flags (int argc, char *argv[]) {
    Flags flags (argc, argv);
}

void test_to_int () {
    int argc = 3;
    char *argv[] = {"./zailton", "--a=12", "-b=15"};
    Flags flags (argc, argv);
    assert (flags.getFlagToInt("a")==12);
    assert (flags.getFlagToInt("b")==15);
}

void test_to_double () {
    int argc = 3;
    char *argv[] = {"./zailton", "--a=12", "-b=15"};
    Flags flags (argc, argv);
    assert (flags.getFlagToDouble("a")==12);
    assert (flags.getFlagToDouble("b")==15);
}

void test_has_flag () {
    int argc = 3;
    char *argv[] = {"./zailton", "--a=12", "-b=15"};
    Flags flags (argc, argv);
    assert (flags.hasFlag("a"));
    assert (flags.hasFlag("b"));
    assert (!flags.hasFlag("c"));
    assert (!flags.hasFlag("zailton"));
}

int main (int argc, char* argv[]) {
    test_flags (argc, argv);
    test_to_int();
}