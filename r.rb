while true
  puts `sh student.sh && cat Chess.txt`.split(?\n)[-1]
end
