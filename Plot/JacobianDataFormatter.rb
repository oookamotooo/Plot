text = open("p_eigen.txt")
out = open("p_eigen_out.txt","w")
i=0
text.each do |line|
	if line.include?("$")
		next
	end
	line.gsub!(/\[.+\]/, "")
	line.lstrip!()
	elem = line.split(" ")
	if elem.length() != 3
		next
	end

	str = ""
	elem.each do |e|
		str += !e.include?("i") ? e+"+0i " : e + " "
	end
	out.puts(str.gsub(/[i+]/," ").strip())
	i+=1
end