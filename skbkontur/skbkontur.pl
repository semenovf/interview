#!/usr/bin/perl -w
use strict;
use utf8;
use Text::CSV;

binmode(STDOUT,':utf8');
binmode(STDERR,':utf8');

die "***ERROR: Incomplete arguments\n" unless scalar(@ARGV) == 5;

#
# Индексы полей результирующей таблицы
#
use constant F_NAME    => 0;
use constant F_AGE     => 1;
use constant F_INN     => 2;
use constant F_KPP     => 3;
use constant F_ORGNAME => 4;
use constant F_ORGNUM  => 5;
use constant F_REGDATE => 6;
use constant F_REGNUM  => 7;

use constant F_COUNT   => 8; # Количество полей

# Открыть все файлы, перечисленные в командной строке
open my $fh_root, "<:encoding(utf8)", $ARGV[0] or die $ARGV[0].": $!";
open my $fh_age , "<:encoding(utf8)", $ARGV[1] or die $ARGV[1].": $!";
open my $fh_inn , "<:encoding(utf8)", $ARGV[2] or die $ARGV[2].": $!";
open my $fh_kpp , "<:encoding(utf8)", $ARGV[3] or die $ARGV[3].": $!";
open my $fh_reg , "<:encoding(utf8)", $ARGV[4] or die $ARGV[4].": $!";

my $csv = Text::CSV->new( {sep_char => ",", escape_char => "\"", binary => 0 } )
        or die "Failed use CSV: ".Text::CSV->error_diag();

my @table = ();
my $index_name = {};
my $index_inn  = {};
my $index_kpp  = {};
my $index_reg  = {};

#### Список А ####

$csv->getline($fh_root); # Пропустить заголовок
my $lineno = 1;

while (my $row = $csv->getline($fh_root)) {
    ++$lineno;
    $row->[0] =~ m/(\w+)\s+(\w+)\s+(\w+)/ or do { # Формат: ФИО
        warn $ARGV[0].": Некорректное значение поля 'ФИО' в строке " . $lineno . "\n";
        next;
    };

    my $name = "$1 $2 $3";

    $row->[1] =~ m/(\d+)\-(\d+)/ or do { # Формат: ИНН-КПП
        warn $ARGV[0].": Некорректное значение поля 'ИНН-КПП' в строке " . $lineno . "\n";
        next;
    };

    my $inn = $1;
    my $kpp = $2;

    $row->[2] =~ m/(\d\d)\.(\d\d)\.(\d\d\d\d)/ or do { # Формат: 31.12.2017
        warn $ARGV[0].": Некорректное значение поля 'Дата регистрации' в строке " . $lineno . "\n";
        next;
    };

    my $regdate = int("$3$2$1"); # 20171231

    my $record = [];
    @$record = ('') x F_COUNT;
    $record->[F_NAME]    = $name;
    $record->[F_INN]     = $inn;
    $record->[F_KPP]     = $kpp;
    $record->[F_REGDATE] = $regdate;

    push(@table, $record);

    $index_name->{$name}   = \$record;
    $index_inn->{$inn}     = \$record;
    $index_kpp->{$kpp}     = \$record;
    $index_reg->{$regdate} = \$record;
}

$csv->eof or $csv->error_diag();
close $fh_root; 

#### Список Б ####

$csv->getline($fh_age); # Пропустить заголовок
$lineno = 1;

while (my $row = $csv->getline($fh_age)) {
    ++$lineno;
    $row->[0] =~ m/(\w+)\s+(\w+)\s+(\w+)/ or do { # Формат: ИОФ
        warn $ARGV[1].": Некорректное значение поля 'ИОФ' в строке " . $lineno . "\n";
        next;
    };

    my $name = "$3 $1 $2";

    $row->[1] =~ m/(\d+)/ or do { # Формат: ФИО
        warn $ARGV[1].": Некорректное значение поля 'Возраст' в строке " . $lineno . "\n";
        next;
    };

    my $age = int($1);

    my $record_ref = $index_name->{$name};

    if (! defined($record_ref)) {
        my $record = [];
        @$record = ('') x F_COUNT;
        $record->[F_NAME] = $name;
        $record->[F_AGE]  = $age;
        push(@table, $record);

        $index_name->{$name} = \$record;
    } else {
        $$record_ref->[F_AGE] = $age;
    }
}

$csv->eof or $csv->error_diag();
close $fh_age;

#### Список В ####

$csv->getline($fh_inn); # Пропустить заголовок
$lineno = 1;

while (my $row = $csv->getline($fh_inn)) {
    ++$lineno;

    $row->[0] =~ m/(\d+)/ or do { # Формат: ИНН
        warn $ARGV[2].": Некорректное значение поля 'ИНН' в строке " . $lineno . "\n";
        next;
    };

    my $inn = $1;
    my $orgname = $row->[1];

    my $record_ref = $index_inn->{$inn};

    if (! defined($record_ref)) {
        my $record = [];
        @$record = ('') x F_COUNT;
        $record->[F_INN]    = $inn;
        $record->[F_ORGNAME] = $orgname;
        push(@table, $record);

        $index_inn->{$inn} = \$record;
    } else {
        $$record_ref->[F_INN]   = $inn;
        $$record_ref->[F_ORGNAME] = $orgname;
    }
}

$csv->eof or $csv->error_diag();
close $fh_inn;

#### Список Г ####

$csv->getline($fh_kpp); # Пропустить заголовок
$lineno = 1;

while (my $row = $csv->getline($fh_kpp)) {
    ++$lineno;

    $row->[0] =~ m/(\d+)/ or do { # Формат: КПП
        warn $ARGV[3].": Некорректное значение поля 'КПП' в строке " . $lineno . "\n";
        next;
    };

    my $kpp = $1;

    $row->[1] =~ m/(\d+)/ or do {
        warn $ARGV[3].": Некорректное значение поля 'Численность организации' в строке " . $lineno . "\n";
        next;
    };

    my $orgnum = int($1);

    my $record_ref = $index_kpp->{$kpp};

    if (! defined($record_ref)) {
        my $record = [];
        @$record = ('') x F_COUNT;
        $record->[F_KPP]    = $kpp;
        $record->[F_ORGNUM] = $orgnum;
        push(@table, $record);

        $index_kpp->{$kpp} = \$record;
    } else {
        $$record_ref->[F_KPP]    = $kpp;
        $$record_ref->[F_ORGNUM] = $orgnum;
    }
}

$csv->eof or $csv->error_diag();
close $fh_kpp;

#### Список Д ####

$csv->getline($fh_reg); # Пропустить заголовок
$lineno = 1;

while (my $row = $csv->getline($fh_reg)) {
    ++$lineno;

    $row->[0] =~ m/(\d\d\d\d)\-(\d\d)\-(\d\d)/ or do { # Формат: 2017-12-31
        warn $ARGV[4].": Некорректное значение поля 'Дата регистрации' в строке " . $lineno . "\n";
        next;
    };

    my $regdate = int("$1$2$3"); # 20171231

    $row->[1] =~ m/(\d+)/ or do {
        warn $ARGV[4].": Некорректное значение поля 'Регистраций в этот день' в строке " . $lineno . "\n";
        next;
    };

    my $regnum = int($row->[1]);

    my $record_ref = $index_reg->{$regdate};

    if (! defined($record_ref)) {
        my $record = [];
        @$record = ('') x F_COUNT;
        $record->[F_REGDATE] = $regdate;
        $record->[F_REGNUM]  = $regnum;
        push(@table, $record);

        $index_reg->{$regnum} = \$record;
    } else {
        $$record_ref->[F_REGDATE] = $regdate;
        $$record_ref->[F_REGNUM]  = $regnum;
    }
}

$csv->eof or $csv->error_diag();
close $fh_reg;

foreach my $r (@table) {
    my $status = $csv->combine(values(@$r));
    print $csv->string(), "\n";
}

0;
