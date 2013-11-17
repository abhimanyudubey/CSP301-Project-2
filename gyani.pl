#Gyani code starts

use List::Util 'max';
use Scalar::Util qw(looks_like_number);
use feature qw(switch);

#Reading the university log to obtain the list of universities
open(DATA, "<univ.log") or die "Couldn't open file university.log, $!";
@universities=<DATA>;
for ($i=0; $i<scalar @universities; $i++)
	{$universities[$i] =~ s/^\s+|\s+$//g;}
close(DATA) || die "Couldn't close file properly";


#keywords permitted along with their respective weights for different universities
%keyword_hash=('clique',[(5,0,0,0,0,0)], 'shortest', [(0,3,3,3,0,0)],'path', [(0,3,3,3,0,0)], 'length', [(0,2,0,0,0,0)], 'list',[(0,0,2,0,0,0)], 'self', [(0,0,0,0,2,0)], 'importance', [(0,0,0,0,3,3)], 'relative', [(0,0,0,0,0,2)], 'size', [(2,0,0,0,0,0)],);

@keywords=keys %keyword_hash;
$string_keywords=join(', ',@keywords);

$choice=0;

#standard output starts
print "\nWelcome to Gyani!\n";

while ($choice!=-1)
{
	print "\nThese are your keywords:\n$string_keywords\n\nInput a query containg one or more keywords along with the relevant universities and ids(if required) or enter q to quit.\n\n";
	
	#input query from user
	chomp($standard_input=<>);
	$var=$standard_input cmp 'q';

	#Quitting the program
	if ($var==0)
		{
			$choice=-1;
			print "\nThank You for using Gyani. Bye!\n";
			last;
		}


	$standard_input =~ s/[?;:!,.'"]//g;    #removing punctuation marks
	@query_words=split(' ', $standard_input);

	#default initialization
	@match_values=(0,0,0,0,0,0);
	$id1=-1;
	$id2=-1;
	$uni1='NULL';
	$uni2='NULL';

	#finding best possible match for query
	for ($i=0; $i<scalar @query_words; $i++)
	{	
		#finding best possible match for input universit(ies)y
		for ($iter=0; $iter< scalar @universities; $iter++)
			{		
				if (($universities[$iter] cmp $query_words[$i])==0)
					{
						if (($uni1 cmp 'NULL')==0)
							{ 
								$uni1=$query_words[$i];
					  		}
						elsif (($uni2 cmp 'NULL')==0)
							{
								$uni2=$query_words[$i];
							}
					
					}			
			}	

		#finding best possible match for input ID(s)
		if (looks_like_number($query_words[$i]))
			{	
				if ($id1==-1)
					{
					 $id1=$query_words[$i];
					}
				elsif ($id2==-1)
					{	
					 $id2=$query_words[$i];
					}
				
			}

		#finding best possible match for input query type
		foreach (@keywords)
		{	if (($_ cmp $query_words[$i])==0)
			{
			$curr_key=$_;		
			@temp=@{$keyword_hash{$curr_key}};

			for ($k=0;$k<scalar @match_values;$k++)
				{
    				$match_values[$k]+= $temp[$k];
				}

			}		
		}	
	}


#assigning weights to number of input parameters
if ($id1==-1)
	{$match_values[3]+=1;}
elsif ($id2==-1)
	{$match_values[0]+=1;
	$match_values[4]+=1;
	$match_values[5]+=1;	
	}
else
	{$match_values[1]+=1;
	$match_values[2]+=1;
	}

#calling Analyzer if match found else giving error message
if (max(@match_values)!=0)
	{$idMax = 0;
    	$match_values[$idMax] > $match_values[$_] or $idMax = $_ for 1 .. $#match_values;	
	$choice=$idMax+1;
	print "\nMatch found. Query Number:$choice. Connecting to Analyzer...\n";
	$global_id1=join('',$uni1, $id1);
	$global_id2=join('',$uni2, $id2);
	system('./Analyzer', $choice, $global_id1, $global_id2);
	}
else
	{
		print "Invalid input. Try again. \n\n";
	}

	$choice=0;
}

#Gyani code completes
