package com.fishjam.study.java;

/****************************************************************************************
 * ������(JDK1.4������ģ�湦�ܣ���ʵ�ּ�����C++��ͬ)
 *   ArrayList(���Vector)
 *   BitSet -- ����ѹ���洢����Bool��Ϣ(��ʡ�ռ䣬���ٶ�����)��ȱʡ��64bits
 *   Collection 
 *   HashMap(���HashTable)��
 *   HashSet(���ҿ�)
 *   Map(key-value��) -- �ɷ���һ����key�γɵ�set��һ����value�γɵ�Collection
 *   TreeMap(��������²����򣬿�ͨ��SubMap����tree�еĲ���)�� ��Ҫ����Ԫ�أ������� entrySetȡ�� keySet���������ܸ��ߡ�
 *     ������룺 Iterator set = symbolMap.keySet().iterator(); while(set.hasNext()){...}
 *     ��ȷ���룺 for(Entry<Icon,Integer> set:symbolMap.entrySet()) { set.getValue() }
 *   TreeSet(����Set)
 *   WeakHashMap(����WeakReference����ʡ�ռ�)
 *   
 * Ԫ������ 
 *   1.���Ƚ���ʵ�� Comparable �ӿ�(ע�ⲻ�ܼ�дΪ return r1-r2;--���������)�� 
 *   2.�������࣬ʵ�� Comparator �ӿڣ��� Collection.reverseOrder--ϵͳ�ṩ�ķ�������
 *   3.�Զ������hashCode(�����ҵ�λ��)��equals(�ж�Ψһ��)����ͬʱʵ��--��Ϊkey���߷���Set�С�
 *     ��Ҫ����TreeSet/TreeMap������ʵ��Comparable�ӿڡ�Set����������ظ�Ԫ�� equalsʵ��ʱ�������(obj instanceof ����)���ж϶�������� 
 * 
 * HasMap�����ܷ�����Capacity(buckets����)��loadfactor(��������)��size/capacity Խ��˵������Խ�ߣ�Խ���׳��ֳ�ͻ�����ﵽָ��ֵʱ(Ĭ��0.75)���Զ�����
 * UnModifialbeXXX -- ���ֻ���汾������������и��ķ������ã�����׳��쳣 
 * Collections.synchronzedXXX(new YYY) -- ���ɿ��Զ�ͬ��������
****************************************************************************************/
 
public class ContainerStudy {

}
