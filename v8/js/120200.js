var buf = new server.Block_Buffer;
buf.write_int32(2);
buf.write_string("hello");
var num = buf.read_int32();
server.Print(num);
var name = buf.read_string();
server.Print(name);

for (var i=0; i<5; i++)
{
  	server.Print(++i);
}