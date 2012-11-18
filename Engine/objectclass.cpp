#include "objectclass.h"
#include "geometryclass.h"


ObjectClass::ObjectClass()
{
}

void ObjectClass::Render(ID3D11Device *dev, ID3D11DeviceContext *devcon, ID3D11RenderTargetView *backbuffer, IDXGISwapChain *swapchain, ID3D11ShaderResourceView *pTexture)
{


        // select which vertex buffer to display
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;
        devcon->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);

		// set the index buffer
		devcon->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);
       
		// select which primtive type we are using
        devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		
        devcon->PSSetShaderResources(0, 1, &texturemap);
        devcon->PSSetShaderResources(1, 1, &alphamap);


        // draw the vertex buffer to the back buffer
        devcon->DrawIndexed(numIndices, 0, 0);


}
