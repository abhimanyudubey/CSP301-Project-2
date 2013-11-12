use List::Util 'max';
use Scalar::Util qw(looks_like_number);
use feature qw(switch);
#se List::MoreUtils 'pairwise';

%keyword_hash=('clique',[(5,0,0,0,0,0)], 'shortest', [(0,3,3,3,0,0)],'path', [(0,3,3,3,0,0)], 'length', [(0,2,0,0,0,0)], 'list',[(0,0,2,0,0,0)], 'self', [(0,0,0,0,2,0)], 'importance', [(0,0,0,0,3,3)], 'relative', [(0,0,0,0,0,2)], 'size', [(2,0,0,0,0,0)],);
	#@args=(1);
	#system('./Analyzer', @args);
@keywords=keys %keyword_hash;
#%hashtest=('a', [(1,0,0,3,0,0)], 'b', [(3,2,4,1,4,5)]);
#print @{$hashtest{'a'}};
$string_keywords=join(', ',@keywords);
@universities=("IITD", "IITB", "IITK");
#TODO: get universities from PART A 
$choice=0;
print "Welcome to Gyani!\n\n";

while ($choice!=-1)
{
	print "These are your keywords:\n$string_keywords\nInput an element from the keywords or enter q
	to quit.\n";
	chomp($standard_input=<>);
	$var=$standard_input cmp 'q';
	if ($var==0)
		{
			$choice=-1;
			print "Thank You for using Gyani. Bye!\n";
			last;
		}
	$standard_input =~ s/[?;:!,.'"]//g;    #removing punctuation marks
	@query_words=split(' ', $standard_input);
	@match_values=(0,0,0,0,0,0);
	$id1=-1;
	$id2=-1;
	$uni1='NULL';
	$uni2='NULL';
	for ($i=0; $i<scalar @query_words; $i++)
	{	
		for ($iter=0; $iter< scalar @universities; $iter++)
			{		#print "$universities[$iter] univesity iter \n";
				if (($universities[$iter] cmp $query_words[$i])==0)
					{
						if (($uni1 cmp 'NULL')==0)
							{ $uni1=$query_words[$i];
					  		print "$uni1 first uni\n";
							}
						elsif (($uni2 cmp 'NULL')==0)
							{$uni2=$query_words[$i];
							print "$uni2 sec uni";
							}
					
					}			
			}	
		if (looks_like_number($query_words[$i]))
			{	
				if ($id1==-1)
					{ $id1=$query_words[$i];
					  print "$id1 first id\n";
					}
				elsif ($id2==-1)
					{$id2=$query_words[$i];
					print "$id2 sec id";
					}
				
			}
		foreach (@keywords)
		{	if (($_ cmp $query_words[$i])==0)
			{
			#@match_values = pairwise { $a + $b } @match_values, @{$keyword_hash{$_}};
			$curr_key=$_;		
			@temp=@{$keyword_hash{$curr_key}};

			for ($k=0;$k<scalar @match_values;$k++){
    				$match_values[$k]+= $temp[$k];
			}

			}		
		}	
		
=pod
	$var=$standard_input cmp $keywords[$i];
		if ($var==0)
		{
			$choice=$i+1;
			print "Match found: $choice\n";
			#TODO: call to function and print to standard output. answer=__
			print "Answer to your query is: \n\n";
			last;
		}
=cut
	}

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
print "@match_values yes yes \n";
	if (max(@match_values)!=0)
	
	{$idMax = 0;
    $match_values[$idMax] > $match_values[$_] or $idMax = $_ for 1 .. $#match_values;	
	$choice=$idMax+1;
	print "Match found: $choice\n";
	#TODO: call to function and print to standard output. answer=__
	#$global_id1=system('./Converter', 2, $id1, $uni1);
	#$global_id2=system('./Converter', 2, $id2, $uni2);
	$global_id1=join('',$uni1, $id1);
	$global_id2=join('',$uni2, $id2);
	system('./Analyzer', $choice, $global_id1, $global_id2);

	#print "Answer to your query is: \n\n";
	
	}
	else
	{
		print "Invalid input. Try again. \n\n";
	}

	$choice=0;
}
