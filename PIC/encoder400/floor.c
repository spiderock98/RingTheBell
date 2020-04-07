float32 FLOOR(float32 x)
{
   float32 y, res;
   unsigned int16 l;
   int1 s;

   s = 0;
   y = x;

   if (x < 0)
   {
      s = 1;
      y = -y;
   }

   if (y <= 32768.0)
      res = (float32)(unsigned int16)y;

   else if (y < 10000000.0)
   {
      l = (unsigned int16)(y * 0.000030517578125);
      y = 32768.0 * (y * 0.000030517578125 - (float32)l);
      res = 32768.0 * (float32)l;
      res += (float32)(unsigned int16)y;
   }

   else
      res = y;

   y = y - (float32)(unsigned int16)y;

   if (s)
      res = -res;

   if (y != 0)
   {
      if (s == 1)
         res -= 1.0;
   }
   if (x == 0)
      res = 0;

   return (res);
}