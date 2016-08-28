for (my $i = 1; $i <= 80; $i++) {
    if ($i % 9 == 0) {
	next;
    }
    my $c = ord 'A';
    $c += ($i % 9) - 1;
    print "#define   ", chr $c, int ($i / 9), "   $i\n";
}
