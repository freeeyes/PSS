--����һ����׼��Lua PassTcp�ű�
--�ṩ�����¼��ķ�װ�ͽ��յ������¼��ķ�װ
--�������������֯��ķ������ݺ���֤��������
--����ӿ�������ƣ���Ҫ�ǿ��ǵ����̲߳������������ٲ���Ҫ���߳���
--������������ݿ鲻Ҫ�ڽű���������������㲻���ͷ�
--Ŀǰ���֧��100K�����ݿ��С
--add by freeeyes

--Ҫ�������ݵ��¼�����PassTCP���ã�����ʵ����ķ������ݷ��
--��ڲ�����nThreadID�ǵ�ǰ�߳�ID��szData����Ϣ����(char* ����)��nLen�ǻ�������󳤶ȣ�nSendIndex�ǵ�ǰ���ӵķ��ʹ���
function LuaTest_SendData(szData, nLen, nThreadID, nSendIndex)
    --������������
	--szDataΪchar* ����Ҫ���Ļ���顣
	--nLen�ǵ�ǰ�������󳤶�
	--���nNextIndex����-1����˵����ֵ����,���򷵻���һ�ε���ʼ��λ��
	
	--���4�ֽڰ�ͷ
	nPacketSize = 10
	nStartIndex  = 0
	nNextIndex  = 0
	nNextIndex = Lua_Tcp_Buffer_In_Int32(szData, nPacketSize, nStartIndex, nLen)
	
	--���2�ֽڵ�������
	--nCommandID = 4096
	--nNextIndex = Lua_Tcp_Buffer_In_Int16(szData, nCommandID, nNextIndex, nLen)
	
	--������ݿ�
	szBuff = '12345678'
	nNextIndex = Lua_Tcp_Buffer_In_Block(szData, szBuff, 8, nNextIndex, nLen)
	
	--������뷵��������ݿ�ĳ��ȣ���PassTCP֪�����������ܳ���
	return nNextIndex
end



