
Simulator::Simulator() : M1(0, 10, 10, 10, A0, &Serial)
  {
    
  }
  void Simulator::SendData()
  {
    M1.Hello();
  }
  
